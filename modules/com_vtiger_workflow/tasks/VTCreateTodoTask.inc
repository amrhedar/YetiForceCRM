<?php
/*+**********************************************************************************
 * The contents of this file are subject to the vtiger CRM Public License Version 1.0
 * ("License"); You may not use this file except in compliance with the License
 * The Original Code is:  vtiger CRM Open Source
 * The Initial Developer of the Original Code is vtiger.
 * Portions created by vtiger are Copyright (C) vtiger.
 * All Rights Reserved.
 ************************************************************************************/
require_once('include/Webservices/Utils.php');
require_once("include/Webservices/VtigerCRMObject.php");
require_once("include/Webservices/VtigerCRMObjectMeta.php");
require_once("include/Webservices/DataTransform.php");
require_once("include/Webservices/WebServiceError.php");
require_once 'include/Webservices/ModuleTypes.php';
require_once('include/Webservices/Create.php');
require_once 'include/Webservices/DescribeObject.php';
require_once 'include/Webservices/WebserviceField.php';
require_once 'include/Webservices/EntityMeta.php';
require_once 'include/Webservices/VtigerWebserviceObject.php';

require_once("modules/Users/Users.php");

class VTCreateTodoTask extends VTTask{
	public $executeImmediately = true;

	public function getFieldNames(){return array('todo', 'description', 'time', 'days_start', 'days_end', 'status', 'priority', 'days', 'direction_start', 'datefield_start', 'direction_end', 'datefield_end', 'sendNotification', 'assigned_user_id', 'days', 'doNotDuplicate');}

	function getAdmin(){
		$user = Users::getActiveAdminUser();
		global $current_user;
		$this->originalUser = $current_user;
		$current_user = $user;
		return $user;
	}

	public function doTask($entityData){
		if(!vtlib_isModuleActive('Calendar')) {
			return;
		}
		global $adb, $log, $current_user;
		$userId = $entityData->get('assigned_user_id');
		if($userId===null){
			$userId = vtws_getWebserviceEntityId('Users', 1);
		}
		$moduleName = $entityData->getModuleName();
		$adminUser = $this->getAdmin();
		$id = $entityData->getId();
		if($this->doNotDuplicate == 'true'){
			$parts = explode('x', $id);
			$entityId = $parts[1];
			if($moduleName=='Contacts'){
				$sql = 'SELECT count(vtiger_activity.activityid) AS num FROM vtiger_activity 
					INNER JOIN vtiger_crmentity ON vtiger_crmentity.crmid = vtiger_activity.activityid
					LEFT JOIN vtiger_cntactivityrel ON vtiger_cntactivityrel.activityid = vtiger_activity.activityid 
					WHERE vtiger_crmentity.deleted = 0 AND vtiger_cntactivityrel.contactid = ? AND vtiger_activity.activitytype = ? AND vtiger_activity.subject = ?';
			}else{
				$sql = 'SELECT count(vtiger_activity.activityid) AS num FROM vtiger_activity 
					INNER JOIN vtiger_crmentity ON vtiger_crmentity.crmid = vtiger_activity.activityid
					LEFT JOIN vtiger_seactivityrel ON vtiger_seactivityrel.activityid = vtiger_activity.activityid 
					WHERE vtiger_crmentity.deleted = 0 AND vtiger_seactivityrel.crmid = ? AND vtiger_activity.activitytype = ? AND vtiger_activity.subject = ?';
			}
			$duplicates = $adb->pquery($sql, [$entityId, 'Task', $this->todo]);
			$num = $adb->query_result_raw($duplicates, 0, 'num');
			if($num > 0){
				$log->warn(__CLASS__ . '::' . __METHOD__ . ': To Do was ignored because a duplicate was found.'.$this->todo);
				return;
			}
		}
		//
		$baseDateStart = $entityData->get($this->datefield_start);
		if($baseDateStart == '') {
			$baseDateStart = date('Y-m-d');
		}
		$time = explode(' ',$baseDateStart);
		if(count($time) < 2) {
            $timeWithSec = Vtiger_Time_UIType::getTimeValueWithSeconds($this->time);
            $dbInsertDateTime = DateTimeField::convertToDBTimeZone($baseDateStart.' '.$timeWithSec);
            $time = $dbInsertDateTime->format('H:i:s');
		} else {
			$time = $time[1];
		}
		preg_match('/\d\d\d\d-\d\d-\d\d/', $baseDateStart, $match);
		$baseDateStart = strtotime($match[0]);
		//
		$baseDateEnd = $entityData->get($this->datefield);
		if($baseDateEnd == '') {
			$baseDateEnd = date('Y-m-d');
		}
		$time = explode(' ',$baseDateEnd);
		if(count($time) < 2) {
            $timeWithSec = Vtiger_Time_UIType::getTimeValueWithSeconds($this->time);
            $dbInsertDateTime = DateTimeField::convertToDBTimeZone($baseDateEnd.' '.$timeWithSec);
            $time = $dbInsertDateTime->format('H:i:s');
		} else {
			$time = $time[1];
		}
		preg_match('/\d\d\d\d-\d\d-\d\d/', $baseDateEnd, $match);
		$baseDateEnd = strtotime($match[0]);
		
        $date_start = strftime('%Y-%m-%d', $baseDateStart+$this->days_start*24*60*60*(strtolower($this->direction_start)=='before'?-1:1));
		$due_date = strftime('%Y-%m-%d', $baseDateEnd+$this->days_end*24*60*60*(strtolower($this->direction_end)=='before'?-1:1));
		// Added to check if the user/group is active
		if(!empty($this->assigned_user_id)) {
			$userExists = $adb->pquery('SELECT 1 FROM vtiger_users WHERE id = ? AND status = ?', array($this->assigned_user_id, 'Active'));
			if($adb->num_rows($userExists)) {
				$assignedUserId = vtws_getWebserviceEntityId('Users', $this->assigned_user_id);
				$userId = $assignedUserId;
			} else {
				$groupExist = $adb->pquery('SELECT 1 FROM vtiger_groups WHERE groupid = ?', array($this->assigned_user_id));
				if($adb->num_rows($groupExist)) {
					$assignedGroupId = vtws_getWebserviceEntityId('Groups', $this->assigned_user_id);
					$userId = $assignedGroupId;
				}
                else{ 
                    if($this->assigned_user_id == 'copyParentOwner'){ 
                        $userId = $entityData->get('assigned_user_id'); 
                    } 
                }
			}
		}

		$fields = array(
			'activitytype'=>'Task',
			'description'=>$this->description,
			'subject'=>$this->todo,
			'taskpriority'=>$this->priority,
			'taskstatus'=>$this->status,
			'assigned_user_id'=>$userId,
			'time_start'=> $time,
			'sendnotification'=>($this->sendNotification!='' && $this->sendNotification!='N')?
					true: false,
			'date_start'=>$date_start,
			'due_date'=>$due_date,
			'visibility'=>'Private',
			'eventstatus'=>''
		);
		if($moduleName=='Contacts'){
			$fields['contact_id'] = $id;
		}else{
			$data = vtws_describe('Calendar', $adminUser);
			$fieldInfo = $data['fields'];
			foreach($fieldInfo as $field){
				if($field['name']=='parent_id'){
					$parentIdField = $field;
				}
			}
			$refersTo = $parentIdField['type']['refersTo'];

			if(in_array($moduleName, $refersTo)){
				$fields['parent_id'] = $id;
			}
		}

		$todo = vtws_create('Calendar', $fields, $adminUser);
		$todoIdDetails = vtws_getIdComponents($todo['id']);
		$entityIdDetails = vtws_getIdComponents($id);

		relateEntities(CRMEntity::getInstance($moduleName), $moduleName, $entityIdDetails[1], 'Calendar', $todoIdDetails[1]);

		global $current_user;
		$current_user = $this->originalUser;
	}

	static function conv12to24hour($timeStr){
		$arr = array();
		preg_match('/(\d{1,2}):(\d{1,2})(am|pm)/', $timeStr, $arr);
		if($arr[3]=='am'){
			$hours = ((int)$arr[1]) % 12;
		}else{
			$hours = ((int)$arr[1]) % 12 + 12;
		}
		return str_pad($hours, 2, '0', STR_PAD_LEFT).':'.str_pad($arr[2], 2, '0', STR_PAD_LEFT);
	}

	public function getTimeFieldList() {
		return array('time');
	}

}
?>
