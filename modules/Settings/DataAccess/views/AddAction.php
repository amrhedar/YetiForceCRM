<?php
/*+***********************************************************************************************************************************
 * The contents of this file are subject to the YetiForce Public License Version 1.1 (the "License"); you may not use this file except
 * in compliance with the License.
 * Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.
 * See the License for the specific language governing rights and limitations under the License.
 * The Original Code is YetiForce.
 * The Initial Developer of the Original Code is YetiForce. Portions created by YetiForce are Copyright (C) www.yetiforce.com. 
 * All Rights Reserved.
 *************************************************************************************************************************************/
Class Settings_DataAccess_AddAction_View extends Settings_Vtiger_Index_View {
    public function preProcess(Vtiger_Request $request) {
        parent::preProcess($request);
    }
    public function process(Vtiger_Request $request) {
        $qualifiedModuleName = $request->getModule(false);
        $moduleName = $request->getModule();
        $baseModule = $request->get('base_module');
        $id = $request->get('id');
		
        $viewer = $this->getViewer($request);
        $viewer->assign('MODULE_NAME', $moduleName);
		$viewer->assign('STEP', 3);
		$viewer->assign('BASE_MODULE', $baseModule);
		$viewer->assign('TPL_ID', $id);
        $viewer->assign('ACTIONS_LIST', Settings_DataAccess_Module_Model::listAccesDataDirector($baseModule));
		$viewer->assign('QUALIFIED_MODULE', $qualifiedModuleName);
        $viewer->assign('DOCUMENT_LIST', $qualifiedModuleName);
        echo $viewer->view('AddAction.tpl', $qualifiedModuleName, true);
    }
}