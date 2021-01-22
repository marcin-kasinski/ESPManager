<h2><a id="content" name="content">Status</a></h2>

<fieldset class="cbi-section">
	<legend>System</legend>

	<table width="100%" cellspacing="10">
		<tbody><tr><td width="33%">Hostname</td><td id="hostname"></td></tr>
		<tr><td width="33%">Model</td><td>ESP</td></tr>
		<tr><td width="33%">Firmware Version</td><td><text id="firmware_version2"></text> ( spiffs version: <text id="spiffs_version2"></text> )</td></tr>
		<tr><td width="33%">Local Time</td><td id="localtime"></td></tr>
		<tr><td width="33%">Uptime</td><td id="uptime"></td></tr>		
		<tr><td width="33%">Sunrise / Sunset</td><td><text id="sunrise"></text> / <text id="sunset"></text> ( update time: <text id="sunset_update_time"></text> )</td></tr>
	</tbody></table>
</fieldset>

<fieldset class="cbi-section">
	<legend>Administration widgets</legend>

	<!------------------>


	<table width="100%" cellspacing="10" id='tableadminwidgets'>
	<tbody>
	</tbody></table>


		<!---------------------------------------->

	</fieldset>

<script>


function processRGBLedState() 
{	

var rgbled_state=getRuntimeValueByKey('rgbled.state');
var rgbled_internalName="RGB Led";


if (rgbled_state=="255" || rgbled_state=="0")
	{
		setValue("rgbled.switch_label","LED "+rgbled_internalName+" is disabled");
		setValue("rgbled.switch_button","Enable");
		setElementAttribute("rgbled.switch_button","class", "cbi-button cbi-input-apply");		
	}//if
	else 
	{
		setValue("rgbled.switch_label","LED "+rgbled_internalName+" is enabled");
		setValue("rgbled.switch_button","Disable");
		setElementAttribute("rgbled.switch_button","class", "cbi-button cbi-input-reset");	
	}//if

	setValue("rgbled.switch",rgbled_state);

}

function processRelayState(index) 
{	
var relay_state=getRuntimeValueByKey('relay_state'+index);
var relay_internalName=getRuntimeValueByKey('relay'+index+'.internalName');

if (relay_state=="255" || relay_state=="0")
	{
		setValue("relay"+index+".relay_switch_label","Relay "+relay_internalName+" is disabled");
		setValue("relay"+index+".relay_switch_button","Enable");
		setElementAttribute("relay"+index+".relay_switch_button","class", "cbi-button cbi-input-apply");		
	}//if
	else 
	{
		setValue("relay"+index+".relay_switch_label","Relay "+relay_internalName+" is enabled");
		setValue("relay"+index+".relay_switch_button","Disable");
		setElementAttribute("relay"+index+".relay_switch_button","class", "cbi-button cbi-input-reset");	
	}//if

	setValue("relay"+index+".relay_switch",relay_state);

}			


			
function addRGBLedAdminWidget(parentid, idlabel, idinput, url)
	{

//	alert ('addRGBLedAdminWidget');
	addRelayAdminWidget(parentid, idlabel, idinput, url);
	}

function addRelayAdminWidget(parentid, idlabel, idinput, url)
	{

	var table=document.getElementById(parentid);

	// '+url+'
	
	var row = table.insertRow(table.rows.length);
						
var html='<tr><td width="33%">'+
		'<label id="'+idlabel+'" class="cbi-value-title" for="'+idinput+'"></label></td>'+
		'<td><input class="cbi-button cbi-input-reset" id="'+idinput+'_button" value="Disable" type="submit"'+
		'onClick="processAjaxRequest(\'\', \''+url+'\',processAjaxSwitchChange, false); return false ;" >'+
		'<input type="hidden" name="'+idinput+'" id="'+idinput+'" >'+
		'</td></tr>';

row.outerHTML=html;		
				
			}


function updateTemp()
{

var temp= getRuntimeValueByKey('temperature');

var temperature_measure_time= getRuntimeValueByKey('temperature_measure_time');

var total=100;
var percent= parseInt(temp/total*100,10);

var element=document.getElementById("temptotaltxt");

//var timeDate= getRuntimeValueByKey('timeDate');

setElementAttribute("temppercentdiv","style", "background-color:#43b057; width:"+percent+"%; height:15px");

//element.textContent=temp+" <sup>o</sup>C";
element.innerHTML=temp+" <sup>o</sup>C ( " +temperature_measure_time+ " )";
}			
			
			
function addTempAdminWidget(parentid)
	{
	var temp= getRuntimeValueByKey('temperature');
	var temperature_measure_time= getRuntimeValueByKey('temperature_measure_time');
	var table=document.getElementById(parentid);	
	var row = table.insertRow(table.rows.length);
						
var html='<tr><td width="33%">'+
		'<label class="cbi-value-title" for="tempvalue">Actual temperature</label></td>'+
		'<td><b>'+temp+' <sup>o</sup>C</b> ( measure time : ' +temperature_measure_time+ ' )</td></tr>';

row.outerHTML=html;		
				
			}
						
			</script>

<fieldset class="cbi-section">
	<legend>Memory</legend>

	<table width="100%" cellspacing="10">
		<tbody>
		<tr>
		<td width="33%">Total Program Available</td><td id="memtotal">
			<div style="width:200px; position:relative; border:1px solid #999999">
				<div id = "mempercentdiv" style="background-color:#CCCCCC; width:0%; height:15px">
					<div style="position:absolute; left:0; top:0; text-align:center; width:100%; color:#000000">
						<small id="memtotalavail"></small>
					</div>
				</div>
			</div>
		</td>
		</tr>
		<tr>
		<td width="33%">Filesystem Available</td><td id="memtotalSPIFFS">
			<div style="width:200px; position:relative; border:1px solid #999999">
				<div id = "mempercentdivSPIFFS" style="background-color:#CCCCCC; width:0%; height:15px">
					<div style="position:absolute; left:0; top:0; text-align:center; width:100%; color:#000000">
						<small id="memtotalavailSPIFFS"></small>
					</div>
				</div>
			</div>
		</td>
		</tr>
	</tbody></table>
</fieldset>


<fieldset class="cbi-section">
	<legend>Network</legend>

	<table width="100%" cellspacing="10">
		<tbody><tr><td style="vertical-align:top" width="33%">IPv4 WAN Status</td><td>
			<table><tbody><tr>
				<td id="wan4_i" style="width:16px; text-align:center; padding:3px"><img src="img/wifi.png"><br><small>wifi</small></td>
				<td id="wan4_s" style="vertical-align:middle; padding: 3px"><small><strong>Type: </strong>dhcp<br>
				<strong>Hostname: </strong><text id="hostname2" ></text><br>
				<strong>Address: </strong><text id="ip" ></text><br>
				<strong>MAC: </strong><text id="mac" ></text><br>
				<strong>Netmask: </strong><text id="netmask" ></text><br>
				<strong>Gateway: </strong><text id="gateway" ></text><br>
				<strong>DNS: </strong><text id="dns" ></text><br>
			</tr></tbody></table>
		</td></tr>
				
	</tbody></table>
</fieldset>

<fieldset class="cbi-section">
	<legend>Other devices</legend>

	<table class="cbi-section-table" id="other_devices_table">
		<tr class="cbi-section-table-titles" id="otherDevices_CollectingInit" >
			<th class="cbi-section-table-cell">Hostname</th>
			<th class="cbi-section-table-cell">IPv4-Address</th>
			<th class="cbi-section-table-cell">Relays</th>
			<th class="cbi-section-table-cell">Firmware version</th>
			<th class="cbi-section-table-cell">Uptime</th>
			<th class="cbi-section-table-cell">Last seen</th>
			<th class="cbi-section-table-cell">Action</th>
		</tr>
			
	</table>
</fieldset>
	
			<script>


function deleteDevice(index)
{
//alert('XXXXX');
//processAjaxRequest('other_devices_table', '/handleJSONListOtherDevices',processAjaxListOtherDevices);			
processAjaxRequest('other_devices_table', '/deleteotherespdevice?index='+index+'&token='+token,processAjaxListOtherDevices);
}


function processAjaxListOtherDevices(divid, file, injson) 

{

	try {
		var obj = JSON.parse(injson);
} catch(e) {
        alert(e); // error in the above string (in this case, yes)!
    }						

			
		var size = obj.MDNSDevices.length; 
	
		var table=document.getElementById(divid);

		// najpierw usuwa rekordy 
		var tabsize=table.rows.length;

		for (i = 1; i < tabsize; i++) table.deleteRow(1);

		for (i = 0; i < size; i++) 
		{


            /////////////////////
		    var relayssize = obj.MDNSDevices[i].relays.length; 
			
			
			var relay_html="";
			
			

			
			console.log("hostname "+ obj.MDNSDevices[i].hostname.trim()+", relayssize "+relayssize); 

		    for (ii = 0; ii < relayssize; ii++) 
		    {	
		 
		 idlabel='externalrelay_'+obj.MDNSDevices[i].hostname.trim()+'_'+obj.MDNSDevices[i].relays[ii].id+'.relay_switch_label';
		 idinput=obj.MDNSDevices[i].hostname.trim()+'_relay'+obj.MDNSDevices[i].relays[ii].id;
		 url='http://'+obj.MDNSDevices[i].IP.trim()+'/handleJSONSwitchChange?index='+obj.MDNSDevices[i].relays[ii].id+'&token=1';

				
				relay_html=relay_html+'<label id="'+idlabel+'" class="cbi-value-title" for="'+idinput+'"></label><input class="cbi-button cbi-input-reset" id="'+idinput+'_button" value="Disable" type="submit"'+
		'onClick="processAjaxRequest(\'\', \''+url+'\',processAjaxExternalSwitchChange, false); return false ;" >'+
		'<input type="hidden" name="'+idinput+'" id="'+idinput+'" ><br>' ;// +  relay_html+"id:"+obj.MDNSDevices[i].relays[ii].id+", state:"+obj.MDNSDevices[i].relays[ii].id+"<BR>";
				
				//console.log("relay_html "+ relay_html); 

			}

			console.log("relay_html NEW :"+ relay_html); 


            /////////////////////

			spiffs_version_string="";
			
			if (typeof obj.MDNSDevices[i].spiffs_version != "undefined")
			
			{
			
			spiffs_version_string=' (<small style="color:#f38d45"> '+obj.MDNSDevices[i].spiffs_version.trim()+' </small>)';

			}
			///////////			
			
			var row = table.insertRow(i+1);	
			row.outerHTML='<tr class="cbi-section-table-row cbi-rowstyle-1"><td><a href="http://'+obj.MDNSDevices[i].IP.trim()+'">'+obj.MDNSDevices[i].hostname.trim()+'</a></td><td>'+obj.MDNSDevices[i].IP.trim()+'</td>'+
			'<td>'+relay_html+'</td>'+
			'<td>'+obj.MDNSDevices[i].firmware_version.trim() + spiffs_version_string+'</td>'+
			'<td>'+obj.MDNSDevices[i].uptime.trim()+'</td><td>'+obj.MDNSDevices[i].timestamp.trim()+'</td><td style="width:40px">'+
//'<img src="img/edit.gif" title="Edit this device ('+i+')" value="Edit" onclick="editTask('+i+'); return false;"> '+
'<img src="img/remove.gif" title="Delete this device ('+i+')" value="Delete" onclick="deleteDevice('+i+'); return false;">'+
'</td></tr>';						



	    for (ii = 0; ii < relayssize; ii++) 
		    {	
		 
		 host=obj.MDNSDevices[i].hostname.trim();
		 idinput=obj.MDNSDevices[i].relays[ii].id;
		 internalName=obj.MDNSDevices[i].relays[ii].internalName;
		 relaystate=obj.MDNSDevices[i].relays[ii].state;
	

	processExternalRelayState(host, idinput,internalName, relaystate) ;
			}//	    for (ii = 0; ii < relayssize; ii++) 



			
			}	
	
}			


function processAjaxExternalSwitchChange(divid, file, injson) 
{
	console.log('injson '+ injson); 
	//processExternalRelayState(host, idinput, relaystate);
	
		var obj = JSON.parse(injson);
	var size = obj.runtime.length; 
	
				for (i = 0; i < size; i++) 
				{
	
	
	index=obj.runtime[i].n.trim().substring(11);
	
	internalName=obj.runtime[i].internalName;

	
	v=obj.runtime[i].v.trim();
	host=obj.runtime[i].host.trim();
	//console.log('index '+ index); 
	//console.log('v '+ v); 
	//console.log('host '+ host); 
	processExternalRelayState(host, index,internalName, v);

				}//for
}




function processExternalRelayState(host ,index, internalName, relay_state) 
{	
//alert('IP '+ip+' / index= '+ index+' / state='+relay_state);
//console.log("processExternalRelayState :"); 
//console.log("host :"+ host); 
//console.log("index :"+ index); 
//console.log("relay_state :"+ relay_state); 

if (relay_state=="255" || relay_state=="0")
	{
		//setValue(host+'_'+"relay"+index+".relay_switch_label","Relay "+relay_internalName+" is disabled");
		setValue("externalrelay_"+host+'_'+index+".relay_switch_label",internalName+" is disabled ");
		setValue(host+'_'+"relay"+index+"_button","Enable");
		setElementAttribute(host+'_'+"relay"+index+"_button","class", "cbi-button cbi-input-apply");		
	}//if

	else 
	{
		//setValue("externalrelay_"+host+'_'+index+".relay_switch_label","Relay "+relay_internalName+" is enabled");
		setValue("externalrelay_"+host+'_'+index+".relay_switch_label",internalName+" is enabled ");
		setValue(host+'_'+"relay"+index+"_button","Disable");
		setElementAttribute(host+'_'+"relay"+index+"_button","class", "cbi-button cbi-input-reset");	
	}//if

//	setValue("relay"+index+".relay_switch",relay_state);




}



function updateMemory()
{
var totalmem= getRuntimeValueByKey('totalmem');
var sketchsize= getRuntimeValueByKey('sketchsize');

var percent= parseInt(sketchsize/totalmem*100,10);

var element=document.getElementById("memtotalavail");

setElementAttribute("mempercentdiv","style", "background-color:#CCCCCC; width:"+percent+"%; height:15px");
element.textContent=sketchsize+" MB / "+totalmem+" MB ("+percent+"%)";
}			
			
function updateMemorySPIFFS()
{
var totalmemSPIFFS= getRuntimeValueByKey('totalmemSPIFFS');
var usedSPIFFS= getRuntimeValueByKey('usedSPIFFS');

var percent= parseInt(usedSPIFFS/totalmemSPIFFS*100,10);

var element=document.getElementById("memtotalavailSPIFFS");

setElementAttribute("mempercentdivSPIFFS","style", "background-color:#CCCCCC; width:"+percent+"%; height:15px");
element.textContent=usedSPIFFS+" MB / "+totalmemSPIFFS+" MB ("+percent+"%)";
}			

document.addEventListener('DOMContentLoaded', function() {

updateRuntimeValues();

	var relay0pin=getRuntimeValueByKey('relay0.relay_pin');
	var relay1pin=getRuntimeValueByKey('relay1.relay_pin');	
	var relay2pin=getRuntimeValueByKey('relay2.relay_pin');	
	var relay3pin=getRuntimeValueByKey('relay3.relay_pin');	
	var ledenable=getRuntimeValueByKey('rgbled.enable');
	
	var temppin=getRuntimeValueByKey('cbid.network.temperaturesensor.pin');

if (relay0pin!=null && relay0pin>=0)addRelayAdminWidget('tableadminwidgets', 'relay0.relay_switch_label', 'relay0.relay_switch', '/handleJSONSwitchChange?index=0&amp;token='+token);
if (relay1pin!=null && relay1pin>=0)addRelayAdminWidget('tableadminwidgets', 'relay1.relay_switch_label', 'relay1.relay_switch', '/handleJSONSwitchChange?index=1&amp;token='+token);
if (relay2pin!=null && relay2pin>=0)addRelayAdminWidget('tableadminwidgets', 'relay2.relay_switch_label', 'relay2.relay_switch', '/handleJSONSwitchChange?index=2&amp;token='+token);
if (relay3pin!=null && relay3pin>=0)addRelayAdminWidget('tableadminwidgets', 'relay3.relay_switch_label', 'relay3.relay_switch', '/handleJSONSwitchChange?index=3&amp;token='+token);

if (ledenable==1)addRGBLedAdminWidget('tableadminwidgets', 'rgbled.switch_label', 'rgbled.switch', '/handleJSONSwitchChange?index=-1&amp;token='+token);

if (temppin>=0)addTempAdminWidget('tableadminwidgets');

processAjaxRequest('other_devices_table', '/handleJSONListOtherDevices',processAjaxListOtherDevices);			

processRelayState(0);
processRelayState(1);
processRelayState(2);
processRelayState(3);
//dodaje led
processRGBLedState();
updateMemory();	
updateMemorySPIFFS();

setValue("firmware_version2",getRuntimeValueByKey('firmware_version'));
setValue("spiffs_version2",getRuntimeValueByKey('spiffs_version'));
setValue("hostname2",getRuntimeValueByKey('hostname'));




}, false);
	
</script>
	