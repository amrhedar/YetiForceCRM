{*<!--
/*+***********************************************************************************************************************************
 * The contents of this file are subject to the YetiForce Public License Version 1.1 (the "License"); you may not use this file except
 * in compliance with the License.
 * Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.
 * See the License for the specific language governing rights and limitations under the License.
 * The Original Code is YetiForce.
 * The Initial Developer of the Original Code is YetiForce. Portions created by YetiForce are Copyright (C) www.yetiforce.com. 
 * All Rights Reserved.
 *************************************************************************************************************************************/
-->*}
{assign var=FINAL_DETAILS value=$RELATED_PRODUCTS.1.final_details}
<table class="table table-bordered mergeTables">
    <thead>
    <th colspan="6" class="detailViewBlockHeader">
	{vtranslate('LBL_ITEM_DETAILS', $MODULE_NAME)}
    </th>
	</thead>
	<tbody>
    <tr>
		<td>
			<span class="redColor">*</span><b>{vtranslate('LBL_ITEM_NAME',$MODULE_NAME)}</b>
		</td>
		<td>
			<b>{vtranslate('LBL_QTY',$MODULE_NAME)}</b>
		</td>
		<td>
			<b>{vtranslate('LBL_LIST_PRICE',$MODULE_NAME)}</b>
		</td>
		<td>
			<b>{vtranslate('LBL_RBH',$MODULE_NAME)}</b>
		</td>
		<td>
			<b>{vtranslate('LBL_TOTAL',$MODULE_NAME)}</b>
		</td>
		<td>
			<b>{vtranslate('ProductComments',$MODULE_NAME)}</b>
		</td>
    </tr>
    {foreach key=INDEX item=LINE_ITEM_DETAIL from=$RELATED_PRODUCTS}
	<tr>
	    <td>
		<div class="row-fluid">
		    {$LINE_ITEM_DETAIL["productName$INDEX"]}
		</div>
		{if $LINE_ITEM_DETAIL["productDeleted$INDEX"]}
			<div class="row-fluid redColor deletedItem">
				{if empty($LINE_ITEM_DETAIL["productName$INDEX"])}
					{vtranslate('LBL_THIS_LINE_ITEM_IS_DELETED_FROM_THE_SYSTEM_PLEASE_REMOVE_THIS_LINE_ITEM',$MODULE)}
				{else}
					{vtranslate('LBL_THIS',$MODULE)} {$LINE_ITEM_DETAIL["entityType$INDEX"]} {vtranslate('LBL_IS_DELETED_FROM_THE_SYSTEM_PLEASE_REMOVE_OR_REPLACE_THIS_ITEM',$MODULE)}
				{/if}
			</div>
		{/if}
		{if !empty($LINE_ITEM_DETAIL["subProductArray$INDEX"])}
		    <div>
			{foreach item=SUB_PRODUCT_NAME from=$LINE_ITEM_DETAIL["subProductArray$INDEX"]}
			    <div>
				{if !empty($SUB_PRODUCT_NAME)}
					- &nbsp; <em>{$SUB_PRODUCT_NAME}</em>
				{/if}
			    </div>
			{/foreach}
		    </div>
		{/if}
	    </td>
	    <td>
			{$LINE_ITEM_DETAIL["qty$INDEX"]}
	    </td>
	    <td>
			<div>
				{$LINE_ITEM_DETAIL["listPrice$INDEX"]}
				{if $MODULE == 'Quotes' || $MODULE == 'SalesOrder'}
					<br />{vtranslate('LBL_PURCHASE',$MODULE)}:<br />
					{$LINE_ITEM_DETAIL["purchase$INDEX"]}
					<br />{vtranslate('LBL_MARGIN',$MODULE)}:<br />
					{$LINE_ITEM_DETAIL["margin$INDEX"]}
					<br />{vtranslate('LBL_MARGINP',$MODULE)}:<br />
					{$LINE_ITEM_DETAIL["marginp$INDEX"]}
				{/if}
			</div>
		</td>
		<td>
		    <div>
			{$LINE_ITEM_DETAIL["rbh$INDEX"]}
		    </div>
		</td>
		<td>
		    <div>
			{$LINE_ITEM_DETAIL["productTotal$INDEX"]}
		    </div>
		</td>
		<td>
			{if !empty($LINE_ITEM_DETAIL["comment$INDEX"]) && $LINE_ITEM_DETAIL["comment$INDEX"] != ' '}
				<div>
					{$LINE_ITEM_DETAIL["comment$INDEX"]}
				</div>
			{/if}
		</td>
	</tr>
	{/foreach}
	</tbody>
	</table>
	<table class="table table-bordered">
	    <tr>
			<td width="83%">
				<span class="pull-right">
				<b>{vtranslate('LBL_GRAND_TOTAL',$MODULE_NAME)}</b>
				</span>
			</td>
			<td>
				<span class="pull-right">
				{$FINAL_DETAILS["grandTotal"]}
				</span>
			</td>
	    </tr>
	    <tr>
			<td width="83%">
				<span class="pull-right">
				<b>{vtranslate('Total Purchase',$MODULE_NAME)}</b>
				</span>
			</td>
			<td>
				<span class="pull-right">
				{$FINAL_DETAILS["total_purchase"]}
				</span>
			</td>
	    </tr>
	    <tr>
			<td width="83%">
				<span class="pull-right">
				<b>{vtranslate('Total margin',$MODULE_NAME)}</b>
				</span>
			</td>
			<td>
				<span class="pull-right">
				{$FINAL_DETAILS["total_margin"]}
				</span>
			</td>
	    </tr>
	    <tr>
			<td width="83%">
				<span class="pull-right">
				<b>{vtranslate('Total margin Percentage',$MODULE_NAME)}</b>
				</span>
			</td>
			<td>
				<span class="pull-right">
				{$FINAL_DETAILS["total_marginp"]}
				</span>
			</td>
	    </tr>
	</table>