<h2><a id="content" name="content">Tasks</a></h2>

<fieldset class="cbi-section">
	<legend>Tasks</legend>

	<table class="cbi-section-table" id="tasks_table">
		<tr class="cbi-section-table-titles" id="otherDevices_CollectingInit" >
			<th class="cbi-section-table-cell">Minute</th>
			<th class="cbi-section-table-cell">Hour</th>
			<th class="cbi-section-table-cell">Day</th>
			<th class="cbi-section-table-cell">Month</th>
			<th class="cbi-section-table-cell">Day of the week</th>
			<th class="cbi-section-table-cell">Function name</th>
			<th class="cbi-section-table-cell">Device</th>
			<th class="cbi-section-table-cell">Actions</th>
		</tr>
			
	</table>
</fieldset>
	
<script>

	    var months = ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'];


	var weekdays = [ "SUN","MON",
    "TUE",
    "WED",
    "THU",
    "FRI",
    "SAT"]; 	

function addTask()
{
	
	var form= document.getElementById('addtask');
	var formData = new FormData( form );
	var request = new XMLHttpRequest();
	request.open("POST", "/addtask");
	request.send(formData);

	clearSelectElements();

	processAjaxRequest('tasks_table', '/handleJSONListSheduledTasks',processAjaxListTasks);
}

function deleteTask(index)
{

processAjaxRequest('tasks_table', '/deletetask?index='+index+'&token='+token,processAjaxListTasks);

}

function initminuteValues(key) 
{
//			 alert('initminuteValues');

		 var selectelement = document.getElementById(key);
		 
//		 alert('initminuteValues / '+selectelement.id);
		 
		  var i, option;
		 
	for ( i = 0; i <= 59; i += 1 ) {

    option = document.createElement( 'option' );
    option.value = option.text = i;
    selectelement.add( option );
	}//for
		 
}

function inithourValues(key) 
{
		 var selectelement = document.getElementById(key);
		 
		  var i, option;
		 
	for ( i = 0; i <= 23; i += 1 ) {

    option = document.createElement( 'option' );
    option.value = option.text = i;
    selectelement.add( option );
	}//for
		 
}
			
function initdayValues(key) 
{
		 var selectelement = document.getElementById(key);
		 
		  var i, option;
		 
	for ( i = 1; i <= 31; i += 1 ) {

    option = document.createElement( 'option' );
    option.value = option.text = i;
    selectelement.add( option );
	}//for
		 
}

function initmonthValues(key) 
{
	
		 var selectelement = document.getElementById(key);
		 
		  var i, option;
		 
	for ( i = 0; i <= 11; i += 1 ) {

	
	var value=months[i];
	
	if (key=='monthvalue3') value=i;

	
    option = document.createElement( 'option' );
	option.value = i;
    option.text = value;
     selectelement.add( option );
	}//for
		 
}

function initdayofweekValues(key) 
{
	
	
		 var selectelement = document.getElementById(key);
		 
		  var i, option;
		 
	for ( i = 0; i <= 6; i += 1 ) {

		var value=weekdays[i];
	
	if (key=='dayofweekvalue3') value=i;

	
    option = document.createElement( 'option' );
    option.value = i;
    option.text = value;
    selectelement.add( option );
	}//for
		 
}

function initCronValues(inpart) 
{
	var key = inpart+'value';

	var selectelement = document.getElementById(key);
	var length =selectelement.options.length;

	addOptionToSelect(inpart+'type', 'EveryTime', 'Every time') ;
	addOptionToSelect(inpart+'type', 'At', 'At') ;
	
	if (inpart!='dayofweek')	addOptionToSelect(inpart+'type', 'Every', 'Every') ;

	addOptionToSelect(inpart+'type', 'Even', 'Even') ;
	addOptionToSelect(inpart+'type', 'Odd', 'Odd') ;
	
	addOptionToSelect(inpart+'type', 'Range', 'Range') ;
	addOptionToSelect(inpart+'type', 'Random', 'Random') ;

	if (length==0 && inpart=='minute') {initminuteValues(key);initminuteValues(key+'2');initminuteValues(key+'3');}
	if (length==0 && inpart=='hour') {inithourValues(key);inithourValues(key+'2');inithourValues(key+'3');} 
	if (length==0 && inpart=='day') {initdayValues(key); initdayValues(key+'2'); initdayValues(key+'3'); }
	if (length==0 && inpart=='month') {initmonthValues(key); initmonthValues(key+'2'); initmonthValues(key+'3'); }
	if (length==0 && inpart=='dayofweek') {initdayofweekValues(key); initdayofweekValues(key+'2'); initdayofweekValues(key+'3'); }	
	
}
			
function updateCronValues(inpart) 
{
	// najpierw sprawdza, czy wartości są:

	var val = getValue(inpart+'type');
	
	var key = inpart+'value';

	// jesli nie randow chowaj wartosci 
	if (val=='Random')
	{
	setElementVisibility(key,true,'display:inline');setElementVisibility(key+'2',true,'display:inline');setElementVisibility(key+'3',true,'display:inline');	

	}
	else if (val=='Range')
	{
	setElementVisibility(key,true,'display:inline');setElementVisibility(key+'2',true,'display:inline');setElementVisibility(key+'3',false);

	}
	else 
	{
	setElementVisibility(key,false);setElementVisibility(key+'2',false);setElementVisibility(key+'3',false);		
	}


	if (val=='EveryTime' ||val=='Even' ||val=='Odd') setElementVisibility(key,false);
	else setElementVisibility(key,true,'display:inline');
	
	if (val=='Every' ||val=='Random' ||val=='Range') 
	{
	removeElementAttribute(key ,'multiple' ) ;
	}
	else setElementAttribute(key,'multiple','1'); 

	var elval = getElement(key);
	
	elval.selectedIndex = 0;

	}

	function replaceNumbesToValuesFomTab(intab , instring, separator, removelast)
	{
		
		var ret=instring ;

//		alert ('replaceNumbesToValuesFomTab in '+ret);

		var pos=instring.indexOf(" ");

		var prefix = '';
//				alert ('pos '+pos);

		if (pos>0) 
			{
			prefix = ret.substring(0,pos+1);
			ret=ret.substring(pos+1,ret.length);

//			alert ('prefix ['+prefix+']');

			}
//		alert ('ret po pos '+ret);

		var numbers = ret.split(separator);

//		alert ('numbers.length '+numbers.length);
		
		var i;
		for (i=0;i<numbers.length-removelast;i++) ret=ret.replace(numbers[i], intab[numbers[i]]);
		
		ret= prefix+ret;
//		alert (ret);
//		alert ('replaceNumbesToValuesFomTab out '+ret);
	
		return ret;
		
	}

//		getRandomAbbr('Random','weekday',weekday_logical,weekday) ;
	
function getRandomAbbr(mode,key ,randommessage,calculated) 
{
	
//		alert('getRandomAbbr in '+randommessage +' calculated '+calculated);
		var numbers = randommessage.substr(7,randommessage.length).split("/");

		var size=numbers.length;

		var value0=numbers[0];
		var value1=numbers[1];
		var value2=numbers[2];
				
		if (key=='month')
		{
		value0=months[numbers[0]];
		value1=months[numbers[1]];

		/*
		var numbers_calculated = calculated.split(",");

		var i;
		for (i=0;i<numbers_calculated.length;i++) calculated=calculated.replace(numbers_calculated[i], months[numbers_calculated[i]]); 
		*/
		

		calculated=replaceNumbesToValuesFomTab(months , calculated, ',', 0);

		
		
		}//if 


		if (key=='weekday')
		{
		value0=weekdays[numbers[0]];
		value1=weekdays[numbers[1]];		
//		alert ('1 calculated months: '+calculated);;
		calculated=replaceNumbesToValuesFomTab(weekdays , calculated, ',', 0);
//		alert ('2 calculated months: '+calculated);;

		
		}//if 
		
		var ret;
		if (mode=='Random') ret='<abbr title="Random from '+value0+' to '+value1+' for '+value2+' times.\nCalculated '+calculated+'">';
		return ret;
	
}	
	
function processAjaxListTasks(divid, file, injson) 
{
		var obj = JSON.parse(injson);
											
		var size = obj.Tasks.length; 
			
		var table=document.getElementById(divid);
		// najpierw usuwa rekordy 

		var tabsize=table.rows.length;

		for (i = 1; i < tabsize; i++) table.deleteRow(1);

		for (i = 0; i < size; i++) 
		{
	
			var row = table.insertRow(i+1);
				
			var minute_logical=obj.Tasks[i].minute_logical.trim();
			var minute=obj.Tasks[i].minute.trim();

			var hour_logical=obj.Tasks[i].hour_logical.trim();
			var hour=obj.Tasks[i].hour.trim();

			var day_logical=obj.Tasks[i].day_logical.trim();
			var day=obj.Tasks[i].day.trim();

			var month_logical=obj.Tasks[i].month_logical.trim();
			var month=obj.Tasks[i].month.trim();

			var weekday_logical=obj.Tasks[i].weekday_logical.trim();
			var weekday=obj.Tasks[i].weekday.trim();

			var minutebefore='';var minuteafter='';
			var hourbefore='';var hourafter='';
			var daybefore='';var dayafter='';
			var monthbefore='';var monthafter='';
			var weekdaybefore='';var weekdayafter='';
			
			if (minute_logical.startsWith("Random"))
			{
			minutebefore=getRandomAbbr('Random','minute',minute_logical,minute) ;
			minuteafter='</abbr>';
			}
			if (hour_logical.startsWith("Random"))
			{
			hourbefore=getRandomAbbr('Random','hour',hour_logical,hour) ;
			hourafter='</abbr>';
			}
			if (day_logical.startsWith("Random"))
			{
			daybefore=getRandomAbbr('Random','day',day_logical,day) ;
			dayafter='</abbr>';
			}
			
//			alert('month_logical '+month_logical+' i '+i+' size '+size);
	
		// jesli to lista miesięcy
			if (month_logical.indexOf(",")!=-1) month_logical=replaceNumbesToValuesFomTab(months , month_logical, ',',0);
	
			if (month_logical.startsWith("Random"))
			{
//			alert ('1 month_logical '+month_logical+' month '+month);
			monthbefore=getRandomAbbr('Random','month',month_logical,month) ;
//			alert ('2 month_logical '+month_logical+' month '+month);
			month_logical=replaceNumbesToValuesFomTab(months , month_logical, '/',1);
//			alert ('3 month_logical '+month_logical+' month '+month);
			monthafter='</abbr>';
			}
			if (month_logical.startsWith("Range")) month_logical=replaceNumbesToValuesFomTab(months , month_logical, '-',0);

			// jesli to lista dni
			if (weekday_logical.indexOf(",")!=-1) weekday_logical=replaceNumbesToValuesFomTab(weekdays , weekday_logical, ',',0);
			if (weekday_logical.startsWith("Random"))
			{
//			alert ('1 weekday_logical '+weekday_logical+' weekday '+weekday);
			weekdaybefore=getRandomAbbr('Random','weekday',weekday_logical,weekday) ;
//			alert ('2 weekday_logical '+weekday_logical+' weekday '+weekday);
			weekday_logical=replaceNumbesToValuesFomTab(weekdays , weekday_logical, '/',1);
			weekdayafter='</abbr>';
			}
			if (weekday_logical.startsWith("Range")) weekday_logical=replaceNumbesToValuesFomTab(weekdays , weekday_logical, '-',0);
			

			function_name_string=obj.Tasks[i].function_name.trim();

			function_name_parameter_extra_string="";
			
			
			if (function_name_string=='Relay on at sunset') function_name_parameter_extra_string=obj.extra_params.sunset;
			else if (function_name_string=='Relay off at sunrise') function_name_parameter_extra_string=obj.extra_params.sunrise;
	
			function_name_parameter_string="";
			
			
			if (obj.Tasks[i].function_name_parameter.trim()!='null' && obj.Tasks[i].function_name_parameter.trim()!='')
			{			
			function_name_parameter_string=' (<small style="color:#f38d45"> '+function_name_parameter_extra_string+" "+obj.Tasks[i].function_name_parameter.trim()+' </small>)';
			}
			
			row.outerHTML='<tr class="cbi-section-table-row cbi-rowstyle-1">'+
			'<td>'+minutebefore+minute_logical+minuteafter+'</td><td>'+hourbefore+hour_logical+hourafter+'</td>'+
			'<td>'+daybefore+day_logical+dayafter+'</td><td>'+monthbefore+month_logical+monthafter+'</td>'+
			'<td>'+weekdaybefore+weekday_logical+weekdayafter+'</td>'+
			'<td>'+function_name_string+function_name_parameter_string+'</td>'+			
			'<td>'+obj.Tasks[i].device_id.trim()+'</td>'+
			
			'<td style="width:40px">'+
'<img src="img/edit.gif" title="Edit this task ('+i+')" value="Edit" onclick="editTask('+i+'); return false;"> '+
'<img src="img/remove.gif" title="Delete this task ('+i+')" value="Delete" onclick="deleteTask('+i+'); return false;">'+
'</td></tr>';
				
		}	//for
}			
			

</script>
	

<form method="POST" id="addtask" name="addtask" action="/addtask" enctype="multipart/form-data">
				<input type="hidden" name="token"/>

<fieldset class="cbi-section">
	<legend>Add new task</legend>

	<table class="cbi-section-table" id="add_edit_table">
		<tr class="cbi-section-table-titles" id="otherDevices_CollectingInit" >

			<th class="cbi-section-table-cell">Minute</th>
			<th class="cbi-section-table-cell">Hour</th>
			<th class="cbi-section-table-cell">Day</th>
			<th class="cbi-section-table-cell">Month</th>
			<th class="cbi-section-table-cell">Day of the week</th>
			<th class="cbi-section-table-cell">Function name</th>
			<th class="cbi-section-table-cell">Function parameter</th>
			<th class="cbi-section-table-cell">Device</th>
			<th class="cbi-section-table-cell">Actions</th>
		</tr>
		<tr  class="cbi-section-table-row cbi-rowstyle-1">
			<td class="cbi-section-table-cell">
				<select class="cbi-input-select" onchange="updateCronValues('minute'); return false" id="minutetype" name="minutetype" ></select>
				<select class="cbi-input-select" id="minutevalue" name="minutevalue"></select>
				<select class="cbi-input-select" id="minutevalue2" name="minutevalue2"></select>
				<select class="cbi-input-select" id="minutevalue3" name="minutevalue3"></select>
			</td>


			<td class="cbi-section-table-cell">
				<select class="cbi-input-select" onchange="updateCronValues('hour'); return false"  id="hourtype" name="hourtype" ></select>
				<select class="cbi-input-select" id="hourvalue" name="hourvalue"></select>			
				<select class="cbi-input-select" id="hourvalue2" name="hourvalue2" ></select>			
				<select class="cbi-input-select" id="hourvalue3" name="hourvalue3" ></select>			
			</td>

			<td class="cbi-section-table-cell">
				<select class="cbi-input-select" onchange="updateCronValues('day'); return false"  id="daytype" name="daytype" ></select>
				<select class="cbi-input-select" id="dayvalue" name="dayvalue"></select>
				<select class="cbi-input-select" id="dayvalue2" name="dayvalue2" ></select>
				<select class="cbi-input-select" id="dayvalue3" name="dayvalue3" ></select>			
			</td>
			
			<td class="cbi-section-table-cell">	
				<select class="cbi-input-select" onchange="updateCronValues('month'); return false"  id="monthtype" name="monthtype" ></select>
				<select class="cbi-input-select" id="monthvalue" name="monthvalue"></select>
				<select class="cbi-input-select" id="monthvalue2" name="monthvalue2" ></select>
				<select class="cbi-input-select" id="monthvalue3" name="monthvalue3" ></select>
			</td>
			
			<td class="cbi-section-table-cell">
				<select class="cbi-input-select" onchange="updateCronValues('dayofweek'); return false"  id="dayofweektype" name="dayofweektype" ></select>
				<select class="cbi-input-select" id="dayofweekvalue" name="dayofweekvalue"></select>
				<select class="cbi-input-select" id="dayofweekvalue2" name="dayofweekvalue2" ></select>
				<select class="cbi-input-select" id="dayofweekvalue3" name="dayofweekvalue3" ></select>
			</td>
			
			<td class="cbi-section-table-cell">
					<select class="cbi-input-select" onchange="onChangeFunctionName(this);" id="function_name" name="function_name">
					<option id="Relay on" value="Relay on" >Relay on</option>
					<option id="Relay off" value="Relay off" >Relay off</option>
					<option id="Reset relay if no internet connection" value="Reset relay if no internet connection" >Reset relay if no internet connection</option>
					<option id="Relay on at sunset" value="Relay on at sunset" >Relay on at sunset</option>
					<option id="Relay off at sunrise" value="Relay off at sunrise" >Relay off at sunrise</option>
					<option id="RGBLed on" value="RGBLed on" >RGBLed on</option>
					<option id="RGBLed off" value="RGBLed off" >RGBLed off</option>
					<option id="RGBLed on at sunset" value="RGBLed on at sunset" >RGBLed on at sunset</option>
					<option id="RGBLed off at sunrise" value="RGBLed off at sunrise" >RGBLed off at sunrise</option>
					</select>
			</td>

			<td class="cbi-section-table-cell">
					<select class="cbi-input-select" id="function_name_parameter" name="function_name_parameter" style="display:none">
					<option value="-2h" >-2h</option>
					<option value="-1h 45m" >-1h 45m</option>
					<option value="-1h 30m" >-1h 30m</option>
					<option value="-1h 15m" >-1h 15m</option>
					<option value="-1h" >-1h</option>
					<option value="-45m" >-45m</option>
					<option value="-30m" >-30m</option>
					<option value="-15m" >-15m</option>
					<option value="+15m" >15m</option>
					<option value="+30m" >+30m</option>
					<option value="+45m" >_45m</option>
					<option value="+1h" >+1h</option>
					<option value="+1h 15m" >+1h 15m</option>
					<option value="+1h 30m" >+1h 30m</option>
					<option value="+1h 45m" >+1h 45m</option>
					<option value="+2h" >+2h</option>
					</select>
			</td>

			
			<td class="cbi-section-table-cell">
					<select class="cbi-input-select" onchange="onChangeUpdateNetProto(this);" id="device_id" name="device_id">
					<option id="1" value="0" >0</option>
					<option id="2" value="1" >1</option>
				</select>
			</td>
			
			<td class="cbi-section-table-cell">
<!--			<input class="cbi-button cbi-button-add" style="width:100px" title="Add this task" value="Add" onclick="joinNetwork(this,' '); return false;" type="submit">-->
			<img src="img/add.gif" title="Add this task" value="Add" onclick="addTask(); return false;">
			</td>

		</tr>
			
	</table>
</fieldset>
		

		

<script>



function onChangeFunctionName(dropdown) {
	
var value = dropdown.options[dropdown.selectedIndex].value;

var element=document.getElementById('function_name_parameter');


if (value =="Relay on at sunset" || value =="Relay off at sunrise")
{
//alert (value);
element.setAttribute('style','display:block');
}
else element.setAttribute('style','display:none');


//if (value=="static") 
//{
//	updateDivFromFile("container.network.lan.general", "proto_static");

//	}
//else if (value=="dhcp") updateDivFromFile("container.network.lan.general", "proto_dhcp");

}




function clearSelectElements()
{

setValue('minutetype','EveryTime');
setValue('hourtype','EveryTime');
setValue('daytype','EveryTime');
setValue('monthtype','EveryTime');
setValue('dayofweektype','EveryTime');

updateCronValues('minute') ;
updateCronValues('hour') ;
updateCronValues('day') ;
updateCronValues('month') ;
updateCronValues('dayofweek') ;

updateToken("addtask");
	
	
}		
		
document.addEventListener('DOMContentLoaded', function() {

updateRuntimeValues();

processAjaxRequest('tasks_table', '/handleJSONListSheduledTasks',processAjaxListTasks);			

initCronValues('minute') ;
initCronValues('hour') ;
initCronValues('day') ;
initCronValues('month') ;
initCronValues('dayofweek') ;

clearSelectElements();


}, false);
	

</script>
	
</form>