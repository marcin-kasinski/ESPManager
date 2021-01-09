<script type="text/javascript">

function LedfieldsSetVisibility(index,visibility)
{
if (visibility<0) visibility=0;
else visibility=1;

	setElementAttribute('relay'+index+'.relay_led_conn_type' ,'data-optional', !visibility) ;
	setElementVisibility('cbi-relay'+index+'-relay_led_conn_type',visibility);
	
}

	function addDivSelect(id, blockid,childName, childid, options, onchange) 
	{
	
	var div=document.getElementById(id);
	var divv=document.getElementById(id).children.length;
	var newDIVchild = document.createElement("div"); 
			
	div.appendChild(newDIVchild); 
		
	div.children[divv].outerHTML='<div class="cbi-value" id="'+blockid+'"><label class="cbi-value-title" >'+
	childName+'</label><div class="cbi-value-field">'+
		'<select class="cbi-input-select" id="'+childid+'" name="'+childid+'">'+
		'</select>'+	
	'</div></div>';		

	var options_size=options.length;	
	var i;
	for (i=0;i<options_size;i++) addOptionToSelect(childid, options[i]['value'], options[i]['text']) ;

	if (typeof(onchange)!='undefined') 	setElementAttribute(childid,'onchange', onchange);

	}
	
	function addDivInput(id, blockid,childName, childid,  datatype, dataoptional, inputhelp, type) 
	{

//		if (id=='proto_dhcp')	alert('childName '+childName);

	var help='';

	if (typeof(inputhelp)!='undefined' && inputhelp!='') help='<br/><div class="cbi-value-description"><span class="cbi-value-helpicon"><img src="img/help.gif" alt="help" /></span>\n'+inputhelp+'</div>';

	var inputtype='text';
	if (typeof(type)!='undefined') inputtype=type;
	
	var div=document.getElementById(id);

//	if (id=='proto_dhcp')	alert('div '+div.innerHTML);

	var divv=document.getElementById(id).children.length;

//	if (id=='proto_dhcp')	alert('divv '+divv);
		
	var newDIVchild = document.createElement("div");  
			
	div.appendChild(newDIVchild); 
	div.children[divv].outerHTML='<div class="cbi-value" id="'+blockid+'"><label class="cbi-value-title" >'+
	childName+'</label><div class="cbi-value-field"><input value="" name="'+childid+'" id="'+childid+'" type="'+inputtype+'" '+
	' class="cbi-input-text" data-type="'+datatype+'" data-optional="'+dataoptional+'" />'+help+'</div></div>';

	}

	function addDivCheckBox(id, childName, childblockid, childid) 
	{
		
	var div=document.getElementById(id);
	var divv=document.getElementById(id).children.length;

	var newDIVchild = document.createElement("div");  
			
	div.appendChild(newDIVchild); 
	div.children[divv].outerHTML='<div class="cbi-value" id="'+childblockid+'" ><label class="cbi-value-title" >'+
	childName+'</label><div class="cbi-value-field"><input value="" name="'+childid+'" id="'+childid+'" type="hidden">'+
	'<input class="cbi-input-checkbox" type="checkbox" id="'+childid+'_checkbox" onchange="setValue(\''+childid+'\',this.checked, \'\');" /></div></div>';
	
//	alert ('XXXYYYZZZZ '+div.children[divv].outerHTML);
	
	}

		
</script>

<div id="maincontent" class="container">

<h2><a id="content" name="content">Hardware configuration</a></h2>

<!-- enctype="multipart/form-data"  -->
<form method="POST" name="cbi" action="/saveconf" onreset="return cbi_validate_reset(this)" onsubmit="return cbi_validate_form(this, 'Some fields are invalid, cannot save values!')">
	<div>
		<script type="text/javascript" src="cbi.js" ></script>
		<input type="hidden" name="cbi.submit" value="1" />
		<input type="hidden" name="token" id="token" value="1" />
		<input type="submit" value="Save" class="hidden" />
	</div>
<div class="cbi-map" id="cbi-network">
	<div class="cbi-map-descr">On this page you can configure the esp device..</div>
	<fieldset class="cbi-section">
		<legend>Common Configuration</legend>
			
	<ul class="cbi-tabmenu">
		<li id="tab.network.lan.general" class="cbi-tab">
			<a onclick="this.blur(); return cbi_t_switch('network.lan', 'general')">General Setup</a>
			<input type="hidden" id="tab.network.lan" name="tab.network.lan" value="general" />
		</li>
		
		<li id="tab.network.lan.physical0" class="cbi-tab-disabled">
			<a onclick="this.blur(); return cbi_t_switch('network.lan', 'physical0')">Relay Settings</a>	
		</li>

		<li id="tab.network.lan.physical1" class="cbi-tab-disabled" style="display:none">
			<a onclick="this.blur(); return cbi_t_switch('network.lan', 'physical1')">Relay2 Settings</a>	
		</li>
		<li id="tab.network.lan.physical2" class="cbi-tab-disabled" style="display:none">
			<a onclick="this.blur(); return cbi_t_switch('network.lan', 'physical2')">Relay3 Settings</a>	
		</li>

		<li id="tab.network.lan.physical3" class="cbi-tab-disabled" style="display:none">
			<a onclick="this.blur(); return cbi_t_switch('network.lan', 'physical3')">Relay4 Settings</a>	
		</li>

		
		<li id="tab.network.lan.rgbled" class="cbi-tab-disabled">
			<a onclick="this.blur(); return cbi_t_switch('network.lan', 'rgbled')">RGB Led Settings</a>	
		</li>
		
		<li id="tab.network.lan.MQTT" class="cbi-tab-disabled">
			<a onclick="this.blur(); return cbi_t_switch('network.lan', 'MQTT')">MQTT Settings</a>
			
		</li>

		<li id="tab.network.lan.temperaturesensor" class="cbi-tab-disabled" style="display:none">
			<a onclick="this.blur(); return cbi_t_switch('network.lan', 'temperaturesensor')">Temperature Sensor Settings</a>
			
		</li>

		<li id="tab.network.lan.advanced" class="cbi-tab-disabled">
			<a onclick="this.blur(); return cbi_t_switch('network.lan', 'advanced')">Advanced Settings</a>
			
		</li>

	</ul>

	<div class="cbi-section-node cbi-section-node-tabbed" id="cbi-network-lan">
	
	<!---->
				
	<div class="cbi-tabcontainer" id="container.network.lan.rgbled" style="display:none">
						
	<script type="text/javascript">
		
	addDivCheckBox('container.network.lan.rgbled','Enable RGB Led','cbi-network-rgbled-enable','rgbled.enable');
	
	addDivInput('container.network.lan.rgbled','cbi-network-rgbled.r','Red','rgbled.r','max(255)', 'false','');	
	addDivInput('container.network.lan.rgbled','cbi-network-rgbled.g','Green','rgbled.g','max(255)', 'false','');
	addDivInput('container.network.lan.rgbled','cbi-network-rgbled.b','Blue','rgbled.b','max(255)', 'false','');
	addDivInput('container.network.lan.rgbled','cbi-network-rgbled.w','White','rgbled.w','max(255)', 'false','');
	addDivInput('container.network.lan.rgbled','cbi-network-rgbled.wa','Warm','rgbled.wa','max(255)', 'false','');

	</script>


	
	</div>
	<script type="text/javascript">cbi_t_add('network.lan', 'rgbled')</script>

<!---->
		

	
<!---->
				
	<div class="cbi-tabcontainer" id="container.network.lan.MQTT" style="display:none">
						
	<script type="text/javascript">
		
	addDivCheckBox('container.network.lan.MQTT','Enable MQTT','cbi-network-mqtt-MQTT_enable','mqtt.MQTT_enable') ;

	var element= getElement('mqtt.MQTT_enable_checkbox') ;
	element.addEventListener("click", function(){

	setValue('mqtt.MQTT_enable',this.checked, '');

	MQTTfieldsSetVisibility(this.checked);
	
	cbi_init();
	
	}, false);

	addDivInput('container.network.lan.MQTT','cbi-network-mqtt-hostname','MQTT server','mqtt.hostname','host', 'false') ;
	addDivInput('container.network.lan.MQTT','cbi-network-mqtt-domoticzhostname','Domoticz server','mqtt.domoticzhostname','host', 'false') ;
	addDivInput('container.network.lan.MQTT','cbi-network-mqtt-domoticzport','Domoticz port','mqtt.domoticzport','min(1)', 'false') ;
	addDivInput('container.network.lan.MQTT','cbi-network-mqtt-domoticzusername','Domoticz username','mqtt.domoticzusername','minlength(1)', 'false') ;
	addDivInput('container.network.lan.MQTT','cbi-network-mqtt-domoticzuserpassword','Domoticz user password','mqtt.domoticzuserpassword','minlength(1)', 'false','','password') ;
	
	</script>


	
	</div>
	<script type="text/javascript">cbi_t_add('network.lan', 'MQTT')</script>

<!---->
		
	<div class="cbi-tabcontainer" id="container.network.lan.temperaturesensor" style="display:none">
	

	<div class="cbi-value" id="cbi-network-temperaturesensor-pin" data-index="2" data-depends="[]"><label class="cbi-value-title" for="cbid.network.temperaturesensor.pin">Temperature sensor Pin</label>
	<div class="cbi-value-field">
	<select class="cbi-input-select" data-update="change" id="cbid.network.temperaturesensor.pin" name="cbid.network.temperaturesensor.pin" size="1"
	
		data-type="min(-1)" data-optional="false" 	>

		<option value="-1" data-index="2" data-depends="[]">Not used</option>
		<option value="4" data-index="3" data-depends="[]">GPIO4 (WeMos D1 Mini)</option>
	</select>
	</div></div>

	
	<!------------------>
	<div class="cbi-value" id="cbi-network-temperaturesensor-domoticzidx" >
	<label class="cbi-value-title" for="cbid.network.temperaturesensor.domoticzidx">Domoticz device idx</label>
	<div class="cbi-value-field">
	<input data-update="change" id="cbid.network.temperaturesensor.domoticzidx" name="cbid.network.temperaturesensor.domoticzidx" class="cbi-input-text " 
	value="" data-type="uinteger" data-optional="true" type="text"" readonly>

	<br />
	<div class="cbi-value-description">
	<span class="cbi-value-helpicon"><img src="img/help.gif" alt="help" /></span>
	Choose domoticz device below ...
	</div>
	
	</div></div>
<!------------------>

	
	
			<!---------------------------------------->
	
<h2 name="content">Domoticz devices</h2>		
<div class="cbi-map">
	<fieldset class="cbi-section">
		<table class="cbi-section-table" style="empty-cells:hide;" id="domoticzscantabletemperaturesensor">

				
<tr id="domoticz_CollectingInit" class="cbi-section-table-row cbi-rowstyle-1" style="vertical-align:middle; text-align:center; padding:3px;"><td class="cbi-value-field" style="width:16px; padding:3px">

<img src="img/signal-none.png"></td>
<td colspan="2" class="cbi-value-field" style="vertical-align:middle; text-align:center; padding:3px;">
<em>...</em></td>
</tr>				
		</table>

		<!-- footer -->
<table id="domoticzscanfooter" class="cbi-section-table" style="empty-cells:hide;" >
<tr class="cbi-section-table-row cbi-rowstyle-1"><td class="cbi-value-field" style="width:16px; padding:3px"></td>
<td class="cbi-value-field" style="vertical-align:middle; text-align:left; padding:3px"></td>
<td style="width:40px"><input class="cbi-button cbi-button-apply" value="Get Domoticz Devices" 
onclick="

var domoticzhostname= getValue('mqtt.domoticzhostname') ;
var domoticzport= getValue('mqtt.domoticzport') ;
var domoticzusername= getValue('mqtt.domoticzusername') ;
var domoticzuserpassword= getValue('mqtt.domoticzuserpassword') ;

var url =encodeURIComponent('http://'+domoticzusername+':'+domoticzuserpassword+'@'+domoticzhostname+':'+ domoticzport+'/json.htm?type=devices&filter=temp&used=true&order=Name');

processAjaxRequest('domoticzscantabletemperaturesensor', '/handleJSONListDomoticzDevices?url='+url,processAjaxDomoticzDevices); return false;

" 
type="submit"></td>
</tr>				
</table>


<!-- footer -->



</fieldset>
</div>

	
<!------------------------------------------>

	</div>
	
	<script type="text/javascript">cbi_t_add('network.lan', 'temperaturesensor')</script>

<!---------------------------------->

	
	<div class="cbi-tabcontainer" id="container.network.lan.physical0" style="display:none">
	
<script type="text/javascript">

function addPhysicalForm(index)
{
	
	

	addDivSelect('container.network.lan.physical'+index,'cbi-relay'+index+'-relay_pin','Relay Pin','relay'+index+'.relay_pin',[
		{
        'text': 'Not used',
        'value': '-1'
        }, 
		{
        'text': 'GPIO26 (Raspberry Pi)',
        'value': '26'
        }, 
		{
        'text': 'GPIO19 (Raspberry Pi)',
        'value': '19'
        }, 
		{
        'text': 'GPIO13 (Raspberry Pi)',
        'value': '13'
        },
		{
        'text': 'GPIO6 (Raspberry Pi)',
        'value': '6'
        },        
		{
        'text': 'GPIO5 (WeMos D1 Mini, Supla, Sonoff TX Relay 2)',
        'value': '5'
        },        
		{
        'text': 'GPIO4 (Shelly Relay 1, Sonoff TX Relay 3)',
        'value': '4'
        },        
		{
        'text': 'GPIO15 (Shelly Relay 2)',
        'value': '15'
        }, 
		{
        'text': 'GPIO13 (Supla GPIO13)',
        'value': '13'
        }, 
		{
        'text': 'GPIO12 (Sonoff TX Relay 1)',
        'value': '12'
        }
, 
		{
        'text': 'GPIO2 (WeMos D1 Mini internal LED)',
        'value': '2'
        }

		],'RelayfieldsSetVisibility('+index+',this.value);	cbi_init();') ;

		addDivInput('container.network.lan.physical'+index,'cbi-relay'+index+'-internalName','Internal Name','relay'+index+'.internalName','minlength(4)', 'false','') ;		
		
		addDivSelect('container.network.lan.physical'+index,'cbi-relay'+index+'-relay_conn_type','Relay connecting type','relay'+index+'.relay_conn_type',[
		{
        'text': 'NC',
        'value': '1'
        }
, 
		{
        'text': 'NO',
        'value': '0'
        }

		]) ;

		
		addDivSelect('container.network.lan.physical'+index,'cbi-relay'+index+'-relay_switch_pin','Relay switch pin','relay'+index+'.relay_switch_pin',[
		{
        'text': 'Not used',
        'value': '-1'
        }
, 
		{
        'text': 'GPIO0 (Sonoff TX Switch 1)',
        'value': '0'
        }
, 
		{
        'text': 'GPIO4 (Supla Sensor 1)',
        'value': '4'
        }
, 
		{
        'text': 'GPIO9 (Sonoff TX Switch 3)',
        'value': '9'
        }
, 
		{
        'text': 'GPIO10 (Sonoff TX Switch 2)',
        'value': '10'
        }
, 
		{
        'text': 'GPIO12 (Supla Button 2)',
        'value': '12'
        }
, 
		{
        'text': 'GPIO14 (Supla Button 1)',
        'value': '14'
        }
, 
		{
        'text': 'GPIO16 (Supla Sensor 2)',
        'value': '16'
        }

		]) ;

		addDivSelect('container.network.lan.physical'+index,'cbi-relay'+index+'-relay_switch_type','Relay switch type','relay'+index+'.relay_switch_type',[
		{
        'text': 'BUTTON',
        'value': '0'
        }
, 
		{
        'text': 'SWITCH',
        'value': '1'
        }

		]) ;

		addDivSelect('container.network.lan.physical'+index,'cbi-relay'+index+'-relay_led_pin','Relay Led Pin','relay'+index+'.relay_led_pin',[
		{
        'text': 'Not used',
        'value': '-1'
        }
		, 
		{
        'text': 'GPIO12 (Sonoff)',
        'value': '13'
        }
		, 
		{
        'text': 'GPIO13 (Sonoff TX Led)',
        'value': '13'
        }
		, 
		{
        'text': 'GPIO2 (D4)',
        'value': '2'
        }

		],'LedfieldsSetVisibility('+index+',this.value);cbi_init();') ;

		addDivSelect('container.network.lan.physical'+index,'cbi-relay'+index+'-relay_led_conn_type','Relay led connecting type','relay'+index+'.relay_led_conn_type',[
		{
        'text': 'high = on',
        'value': '1'
        }
, 
		{
        'text': 'low = on',
        'value': '0'
        }

		]) ;

	addDivCheckBox('container.network.lan.physical'+index,'Enable Amazon Alexa voice control','cbi-relay'+index+'-enablealexa','relay'+index+'.enablealexa') ;

	addDivInput('container.network.lan.physical'+index,   'cbi-relay'+index+'-domoticzidx','Domoticz Device idx','relay'+index+'.domoticzidx','uinteger', 'true','Choose domoticz device below ...') ;

	}


addPhysicalForm(0);

	</script>
	
<h2 name="content">Domoticz devices</h2>		
<div class="cbi-map">
	<fieldset class="cbi-section">
		<table class="cbi-section-table" style="empty-cells:hide;" id="domoticzscantablephysical0">

				
<tr id="domoticz_CollectingInit" class="cbi-section-table-row cbi-rowstyle-1" style="vertical-align:middle; text-align:center; padding:3px;"><td class="cbi-value-field" style="width:16px; padding:3px">

<img src="img/signal-none.png"></td>
<td colspan="2" class="cbi-value-field" style="vertical-align:middle; text-align:center; padding:3px;">
<em>...</em></td>
</tr>				
		</table>

		<!-- footer -->
<table id="domoticzscanfooter" class="cbi-section-table" style="empty-cells:hide;" >
<tr class="cbi-section-table-row cbi-rowstyle-1"><td class="cbi-value-field" style="width:16px; padding:3px"></td>
<td class="cbi-value-field" style="vertical-align:middle; text-align:left; padding:3px"></td>
<td style="width:40px"><input class="cbi-button cbi-button-find" value="Get Domoticz Devices" 
onclick="

var domoticzhostname= getValue('mqtt.domoticzhostname') ;
var domoticzport= getValue('mqtt.domoticzport') ;
var domoticzusername= getValue('mqtt.domoticzusername') ;
var domoticzuserpassword= getValue('mqtt.domoticzuserpassword') ;

//var url =encodeURIComponent('http://'+domoticzusername+':'+domoticzuserpassword+'@'+domoticzhostname+':'+ domoticzport+'/json.htm?type=devices&filter=light&used=true&order=Name');

var url =encodeURIComponent('http://'+domoticzusername+':'+domoticzuserpassword+'@'+domoticzhostname+':'+ domoticzport+'/json.htm?type=command&param=getlightswitches');



processAjaxRequest('domoticzscantablephysical0', '/handleJSONListDomoticzDevices?url='+url,processAjaxDomoticzDevices); return false;

" 
type="submit"></td>
</tr>				
</table>


<!-- footer -->



</fieldset>
</div>

	
<!------------------------------------------>


			
	</div>
	
	<script type="text/javascript">cbi_t_add('network.lan', 'physical0')</script>

	<div class="cbi-tabcontainer" id="container.network.lan.physical1" style="display:none"><script>addPhysicalForm(1);</script></div>
	<script type="text/javascript">cbi_t_add('network.lan', 'physical1')</script>
	<div class="cbi-tabcontainer" id="container.network.lan.physical2" style="display:none"><script>addPhysicalForm(2);</script></div>
	<div class="cbi-tabcontainer" id="container.network.lan.physical3" style="display:none"><script>addPhysicalForm(3);</script></div>
	<script type="text/javascript">cbi_t_add('network.lan', 'physical2');</script>
	<script type="text/javascript">cbi_t_add('network.lan', 'physical3');</script>
	<script type="text/javascript">cbi_t_add('network.lan', 'physical4');</script>

	
			<script  type="text/javascript">



function convertdBmToQuality(dBmString)
{

var dBm=parseInt(dBmString, 10);

	 // dBm to Quality:
    if(dBm <= -100)
        quality = 0;
    else if(dBm >= -50)
        quality = 100;
    else
        quality = 2 * (dBm + 100);
	
	return quality;
}			


function joinNetwork(button, index,name)
{
	var ssid=document.getElementById('cbid.network.lan.wifissid');
	ssid=document.getElementById('cbid.network.lan.wifissid');

	ssid.value=name;
	
	for (i = 0; i < 1000; i++) {
		
	var searchedid="wifinetwork_"+i;
	
	var tr=document.getElementById("wifinetwork_"+i);
		
	if (tr ==null) break;
	tr.parentNode.removeChild(tr);
	
			}//for	
}

			function addWIFINetworkToList( index, name,bssid, rssi, channel, encryptionType )
			{
	
			var quality = convertdBmToQuality(rssi);
						
			var percent=Math.round(quality*100/100);
			
			var img_file ='';

			if (percent <=25  ) 			var img_file ='signal-0-25.png';
			else if (percent <=50 ) 			var img_file ='signal-25-50.png';
			else if (percent <=75 ) 			var img_file ='signal-50-75.png';
			else if (percent >75) 			var img_file ='signal-75-100.png';
			
var table=document.getElementById("wifiscantable");
			
var row = table.insertRow(index);

row.outerHTML='<tr id="wifinetwork_'+index+'" class="cbi-section-table-row cbi-rowstyle-1"><td class="cbi-value-field" style="width:16px; padding:3px">'+

'<abbr title="Signal: '+rssi+' dB / Quality: '+quality+'/80"><img src="img/'+img_file +'"><br><small>'+percent+'%</small></abbr></td>'+
'<td class="cbi-value-field" style="vertical-align:middle; text-align:left; padding:3px">'+
'<big><strong>'+name+'</strong></big><br><strong>Channel:</strong> '+channel+' |<strong>BSSID:</strong>'+bssid+' | <strong>Encryption:</strong> <abbr title="'+encryptionType+'">'+encryptionType+'</abbr></td>'+
'<td style="width:40px">'+
'<input class="cbi-button cbi-button-apply" value="Join network" onclick="joinNetwork(this,'+index+',\''+name+'\'); return false;" type="submit">'+
'</td></tr>';
}
			
function addDomoticzDeviceToList(divid, domoticzidx_element_id ,index, name,HardwareType, Type, idx,TypeImg)
{

var domoticzHost=getRuntimeValueByKey('mqtt.domoticzhostname');
var domoticzPort=getRuntimeValueByKey('mqtt.domoticzport');
//var domoticzHost='192.168.1.151';
//var domoticzPort='8080';

//alert('addDomoticzDeviceToList divid '+divid+' domoticzidx_element_id '+ domoticzidx_element_id +' index '+index+' name '+ name+' HardwareType '+HardwareType+' Type '+ Type+' idx '+idx+' TypeImg '+TypeImg);

var table=document.getElementById(divid);

//alert('addDomoticzDeviceToList 2 '+name + ' table '+table.id);
					
var row = table.insertRow(index);

//alert('addDomoticzDeviceToList 3 '+row.outerHTML+' ----');

row.outerHTML='<tr class="cbi-section-table-row cbi-rowstyle-1">'+
'<td class="cbi-value-field" style="width:16px; padding:3px">'+
'<img src="http://'+domoticzHost+':'+domoticzPort+'/images/'+TypeImg +'.png"><br><small>idx:'+idx+'</small></td>'+
//'</td>'+
'<td class="cbi-value-field" style="vertical-align:middle; text-align:left; padding:3px">'+
'<big><strong>'+name+'</strong></big><br><strong>Type:</strong> '+Type+' | <strong>HardwareType:</strong> '+HardwareType+' </td>'+
//' </td>'+
'<td style="width:40px">'+
'<input class="cbi-button cbi-button-apply" value="Choose this device" onclick="setValue(\''+domoticzidx_element_id+'\',\'' +idx+  '\');cbi_init() ;return false;" type="submit">'+
'</td>'+
'</tr>';

//alert('addDomoticzDeviceToList end');


}
			
function processAjaxDomoticzDevices(divid, file, injson) 
{
				
//				alert('processAjaxDomoticzDevices '+injson);
		var domoticzidx_element_id='';
		
		if (divid=='domoticzscantablephysical0') domoticzidx_element_id='relay0.domoticzidx';
		else if (divid=='domoticzscantabletemperaturesensor') domoticzidx_element_id='cbid.network.temperaturesensor.domoticzidx';

		try {
			var obj = JSON.parse(injson);

} catch(e) {
        alert(e);
}			
									
	var size = obj.result.length; 
	
	var tr=document.getElementById("domoticz_CollectingInit");
	tr.parentNode.removeChild(tr);
		for (i = 0; i < size; i++) 
		{
		var TypeImg='';
		if (obj.result[i].Type.trim()=='Light/Switch') TypeImg='lightbulb';
		else if (obj.result[i].Type.trim()=='Temp') TypeImg='temperature';
//		addDomoticzDeviceToList(divid,domoticzidx_element_id,i,obj.result[i].Name.trim(),obj.result[i].HardwareType.trim(),obj.result[i].Type.trim(),obj.result[i].idx.trim(),obj.result[i].TypeImg.trim());
		addDomoticzDeviceToList(divid,domoticzidx_element_id,i,obj.result[i].Name.trim(),'HardwareType',obj.result[i].Type.trim(),obj.result[i].idx.trim(),TypeImg);
					
		}	
}
			
function processAjaxWifiNetworks(divid, file, injson) 
{
		 try {
			var obj = JSON.parse(injson);

} catch(e) {
        alert(e);
}			
												
			var size = obj.networks.length; 

			//////////////////// usuwa tr collecting data
	var tr=document.getElementById("wifinetwork_CollectingInit");
	if (tr!=null) tr.parentNode.removeChild(tr);
			//////////////////// usuwa tr collecting data
	
			for (i = 0; i < size; i++) 	
			{
			addWIFINetworkToList( i,obj.networks[i].name.trim(),	obj.networks[i].bssid.trim(),obj.networks[i].rssi.trim() ,obj.networks[i].channel.trim() ,obj.networks[i].encryptionType.trim() );
			}			
}			
			
function updateDivFromFile(divid, newdivid) {


var div=document.getElementById(divid);
var newdiv=document.getElementById(newdivid);
			
			div.innerHTML = newdiv.innerHTML;
			   
			updateRuntimeValues();


			
//			cbi_init();

			
}			
			
function onChangeUpdateNetProto(dropdown) {
	
var value = dropdown.options[dropdown.selectedIndex].value;

if (value=="static") 
{
	updateDivFromFile("container.network.lan.general", "proto_static");

	}
else if (value=="dhcp") updateDivFromFile("container.network.lan.general", "proto_dhcp");

}

</script>

	<div class="cbi-tabcontainer" id="container.network.lan.general">

	</div>

	<script type="text/javascript">cbi_t_add('network.lan', 'general')</script>

	<div class="cbi-tabcontainer" id="container.network.lan.advanced" style="display:none"></div>
	
	<script type="text/javascript">

	addDivCheckBox('container.network.lan.advanced','Enable OTA','adv-OTA_enable','adv.OTA_enable') ;
	addDivCheckBox('container.network.lan.advanced','Set discoverable','adv-discoverable','adv.discoverable') ;
	addDivCheckBox('container.network.lan.advanced','Firmware update over network','adv-network_update','adv.network_update') ;
	addDivCheckBox('container.network.lan.advanced','Enable Relay 2 tab','cbi-network-lan-advanced-enablerelay2','adv.enablerelay2') ;
	addDivCheckBox('container.network.lan.advanced','Enable Relay 3 tab','cbi-network-lan-advanced-enablerelay3','adv.enablerelay3') ;
	addDivCheckBox('container.network.lan.advanced','Enable Relay 4 tab','cbi-network-lan-advanced-enablerelay4','adv.enablerelay4') ;
	addDivCheckBox('container.network.lan.advanced','Enable Temperature tab','cbi-network-lan-advanced-enabletempsensor','adv.enabletempsensor') ;

	addDivInput('container.network.lan.advanced','adv-sunsetAPIKey','Sunset API Key','adv.sunsetAPIKey','minlength(4)', 'true','','password') ;
	addDivInput('container.network.lan.advanced','adv-sunsetAPICity','Sunset API City','adv.sunsetAPICity','minlength(4)', 'true') ;

	addDivInput('container.network.lan.advanced','adv-ntpserver','NTP server','adv.ntpserver','host', 'true') ;
	addDivInput('container.network.lan.advanced','adv-web_password','Admin password','adv.web_password','minlength(4)', 'true','','password') ;
	cbi_t_add('network.lan', 'advanced');

	function cbi_t_del(field, dep)
	{

		var obj = (typeof(field) === 'string') ? document.getElementById(field) : field;
	if (obj) {
		var entry
		for (var i=0; i<cbi_d.length; i++) {
			if (cbi_d[i].id == obj.id) {
				entry = cbi_d[i];
				break;
			}//if
		}//for
		
	entry.deps.pop(dep);
		
	}	//if (obj) {

	}


	</script>

		</div>
		<br />
	</fieldset>

<!-- /nsection -->

	<br />
</div>
	<div class="cbi-page-actions">
		
		<div style="float:left">
			<input class="cbi-button cbi-button-link" type="button" value="Back to Overview" onclick="location.href='/'" />
		</div>
		
			<input class="cbi-button cbi-button-apply" type="submit" name="cbi.apply" value="Save &#38; Apply" />
				
	</div></form>

<script>

function refreshTabs()
{
/*
	var relay2pin=getRuntimeValueByKey('relay1.relay_pin');
		if (relay2pin<0) 
		{
		disableConfTab('network.lan.physical1');
		setValue('adv.enablerelay2_checkbox','0');
		}
		
	
		else 
		{	
		enableConfTab('network.lan.physical1');
		setValue('adv.enablerelay2_checkbox','1');	
		}
	    
	*/    
	    //check relays
	    var index=1
	    var relaypin=getRuntimeValueByKey('relay'+index+'.relay_pin');

        //alert('refreshTabs before')
        //alert('for index '+index+' '+getRuntimeValueByKey('relay'+index+'.relay_pin'))

    	//while (relaypin=getRuntimeValueByKey('relay'+index+'.relay_pin')!=null)
	    while (true)
	    {
	    var relaypin=getRuntimeValueByKey('relay'+index+'.relay_pin');
	    //alert('relaypin for index '+index+'='+relaypin)
	    
	    if (relaypin==null || relaypin<0) 
		{

		disableConfTab('network.lan.physical'+index+'');

		setValue('adv.enablerelay'+(index+1)+'_checkbox','0');

		}
		
	
		else if (relaypin!=null  && relaypin>=0) 
		{	

		enableConfTab('network.lan.physical'+index+'');

		setValue('adv.enablerelay'+(index+1)+'_checkbox','1');

		}
	    
	    //alert('relaypin for index ...')
	    
	    index++;
	    if (index>32) break;
	    }// while
        //alert('refreshTabs after')

	//alert('aaa');
	var temp_pin=getRuntimeValueByKey('cbid.network.temperaturesensor.pin');
	//alert('temp_pin '+temp_pin);
	
		if (temp_pin<0) 		
		{
		disableConfTab('network.lan.temperaturesensor');
		setValue('adv.enabletempsensor_checkbox','0');
		}
		else 
		{	
		enableConfTab('network.lan.temperaturesensor');	
		setValue('adv.enabletempsensor_checkbox','1');	
		}
		//alert('aaa END');

}

var confTabContents = {};

function disableConfTab(indexname)
{

	setElementVisibility('tab.'+indexname,0) ;
	//alert('disableConfTab '+indexname);
	
	var relay1container=document.getElementById('container.'+indexname);
	if (relay1container==null) return;

	confTabContents[indexname]=relay1container.innerHTML;

	relay1container.innerHTML='';
	//alert('disableConfTab '+indexname+' END');

}

function enableConfTab(indexname)
{
	
//	alert ('enableConfTab '+indexname);
	setElementVisibility('tab.'+indexname,1,'display: inline');
	
	var relay1container=document.getElementById('container.'+indexname);

//		alert('confTabContents.length '+confTabContents.length);
//		alert('confTabContents[indexname] '+confTabContents[indexname]);
//		alert('relay1container '+relay1container.innerHTML);

	relay1container.innerHTML=confTabContents[indexname];
	updateRuntimeValues();
}

function divReload()
{

	updateDivFromFile("container.network.lan.general", "proto_dhcp");	
	disableConfTab('network.lan.physical1');
	disableConfTab('network.lan.physical2');
	disableConfTab('network.lan.physical3');
	disableConfTab('network.lan.temperaturesensor');
	enableConfTab('network.lan.physical1');
	enableConfTab('network.lan.physical2');
	enableConfTab('network.lan.physical3');
	enableConfTab('network.lan.temperaturesensor');
	
	
updateRuntimeValues();

refreshTabs();

var relay2=document.getElementById('adv.enablerelay2_checkbox');
relay2.onchange=function(){
var checked = this.checked;
if (checked ==false)	disableConfTab('network.lan.physical1');
else if (checked ==true)	enableConfTab('network.lan.physical1');
cbi_init();
};


var relay3=document.getElementById('adv.enablerelay3_checkbox');
relay3.onchange=function(){
var checked = this.checked;
if (checked ==false)	disableConfTab('network.lan.physical2');
else if (checked ==true)	enableConfTab('network.lan.physical2');
cbi_init();
};


var relay4=document.getElementById('adv.enablerelay4_checkbox');
relay4.onchange=function(){
var checked = this.checked;
if (checked ==false)	disableConfTab('network.lan.physical3');
else if (checked ==true)	enableConfTab('network.lan.physical3');
cbi_init();
};


	
	
	var enabletempsensor=document.getElementById('adv.enabletempsensor_checkbox');
	
	enabletempsensor.onchange=function(){
		
	var checked = this.checked;
		
	if (checked ==false)	disableConfTab('network.lan.temperaturesensor');
	
	else if (checked ==true)	enableConfTab('network.lan.temperaturesensor');

	cbi_init();
		
	};
	
	setValue('token',token, '');	

	cbi_init();
	
}

document.addEventListener('DOMContentLoaded', function() {
	divReload();	
}, false);
	
</script>

<!-- DHCP start -->
<div id="proto_dhcp" style="display:none">
		<div class="cbi-value" id="cbi-network-lan-__status" data-index="1" data-depends="[]"><label class="cbi-value-title" for="cbid.network.lan.__status">Status</label>
		<div class="cbi-value-field">
<table>
	<tbody><tr class="cbi-section-table">
		<td></td>
		<td class="cbi-value-field" style="min-width:16px; padding:3px; text-align:center" id="__status-ifc-signal"><img src="img/wifi.png" style="width:16px; height:16px"><br><small>wifi</small></td>
		<td class="cbi-value-field" style="vertical-align:middle; text-align:left; padding:3px" id="__status-ifc-description">

		<strong>Hostname:</strong><text id="hostname"></text><br>
		<strong>IPv4:</strong><text id="ip"></text><br>
		<strong>Mac address:</strong><text id="mac"></text><br>
		<strong>Netmask:</strong><text id="netmask"></text><br>
		<strong>Gateway:</strong><text id="gateway"></text><br>
		<strong>DNS:</strong><text id="dns"></text><br>
		<strong>Uptime:</strong><text id="uptime"></text><br>

		</td>
	</tr>
</tbody></table>
		</div></div>

<script type="text/javascript">

	addDivSelect('proto_dhcp','cbi-network-lan-proto','Protocol','cbid.network.lan.proto',[{
        'text': 'DHCP client',
        'value': 'static'
        }, 
		{
        'text': 'Static address',
        'value': 'static'
        } ],'onChangeUpdateNetProto(this);') ;
	addDivInput('proto_dhcp','cbi-network-lan-wifissid','WIFI SSID','cbid.network.lan.wifissid','maxlength(32)','true','Find network below...') ;
	addDivInput('proto_dhcp','cbi-network-lan-wifipassword','WIFI password','cbid.network.lan.wifipassword','minlength(8)', 'true','','password') ;
	addDivInput('proto_dhcp','cbi-network-lan-hostname','Hostname to send when requesting DHCP','cbid.network.lan.hostname','hostname', 'false') ;
	addDivInput('proto_dhcp','cbi-network-lan-broadcast','Broadcast','cbid.network.lan.broadcast','host','false') ;

	</script>
	
<h2 name="content">Join Network: Wireless Scan</h2>		
<div class="cbi-map">
	<fieldset class="cbi-section">
		<table class="cbi-section-table" style="empty-cells:hide;" id="wifiscantable">
	
<tr id="wifinetwork_CollectingInit" class="cbi-section-table-row cbi-rowstyle-1" style="vertical-align:middle; text-align:center; padding:3px;"><td class="cbi-value-field" style="width:16px; padding:3px">

<img src="img/signal-none.png"></td>
<td colspan="2" class="cbi-value-field" style="vertical-align:middle; text-align:center; padding:3px;">
<em>...</em></td>
</tr>								
		</table>

		<!-- footer -->
<table class="cbi-section-table" style="empty-cells:hide;" >
<tr id="wifinetwork_'+index+'" class="cbi-section-table-row cbi-rowstyle-1"><td class="cbi-value-field" style="width:16px; padding:3px"></td>
<td class="cbi-value-field" style="vertical-align:middle; text-align:left; padding:3px"></td>
<td style="width:40px"><input class="cbi-button cbi-button-find" value="Rescan" onclick="processAjaxRequest('wifiscantable', '/listwifinetworks',processAjaxWifiNetworks); return false;" type="submit"></td>
</tr>				
</table>


<!-- footer -->



</fieldset>
</div>


</div>

<!-- DHCP end -->






<!-- proto_static start -->
<div id="proto_static" style="display:none">
<div class="cbi-value" id="cbi-network-lan-proto" data-index="2" data-depends="[]"><label class="cbi-value-title" for="cbid.network.lan.proto">Protocol</label>

	<div class="cbi-value-field">


		<select class="cbi-input-select" onchange="onChangeUpdateNetProto(this);" data-update="change" onchange="onChangeUpdateNetProto(this);" id="cbid.network.lan.proto" name="cbid.network.lan.proto" size="1">
		<option id="cbid.network.lan.proto-static" value="static" data-index="1" data-depends="[]" selected="selected">Static address</option>
		<option id="cbid.network.lan.proto-dhcp" value="dhcp" data-index="2" data-depends="[]" >DHCP client</option>		
	</select>

		</div></div>

	<div class="cbi-value" id="cbi-network-lan-wifissid" data-index="3" data-depends="[{&quot;cbid.network.lan.proto&quot;:&quot;static&quot;}]">
	<label class="cbi-value-title" for="cbid.network.lan.wifissid">WIFI SSID</label>
	<div class="cbi-value-field">
	<input data-update="change" id="cbid.network.lan.wifissid" name="cbid.network.lan.wifissid" class="cbi-input-text " 
	value="" data-type="maxlength(32)" data-optional="true" type="text">

	<br />
	<div class="cbi-value-description">
	<span class="cbi-value-helpicon"><img src="img/help.gif" alt="help" /></span>
	Find network below...
	</div>
	
	</div></div>

	<div class="cbi-value" id="cbi-network-lan-wifipassword" data-index="3" data-depends="[{&quot;cbid.network.lan.proto&quot;:&quot;static&quot;}]">
	<label class="cbi-value-title" for="cbid.network.lan.wifipassword">WIFI password</label>
	<div class="cbi-value-field">
	<input data-update="change" id="cbid.network.lan.wifipassword" name="cbid.network.lan.wifipassword" class="cbi-input-text " 
	value="" data-type="minlength(8)" data-optional="false" type="password" >
	</div></div>
	
		<div class="cbi-value" id="cbi-network-lan-ipaddr" data-index="4" data-depends="[{&quot;cbid.network.lan.proto&quot;:&quot;static&quot;}]"><label class="cbi-value-title" for="cbid.network.lan.ipaddr">IPv4 address</label>
		<div class="cbi-value-field">

	<input data-update="change" id="cbid.network.lan.ipaddr" name="cbid.network.lan.ipaddr" class="cbi-input-text " value="" data-type="ip4addr" data-optional="false" cbi_datatype="ip4addr" cbi_optional="false" type="text">
	
		</div></div>
		
<div class="cbi-value" id="cbi-network-lan-netmask" data-index="5" data-depends="[{&#34;cbid.network.lan.proto&#34;:&#34;static&#34;}]"><label class="cbi-value-title" for="cbid.network.lan.netmask">IPv4 netmask</label>
		<div class="cbi-value-field">

	<input data-update="change" id="cbid.network.lan.netmask" name="cbid.network.lan.netmask" type="text" class="cbi-input-text" value="" data-type="ip4addr" data-optional="false" data-choices="[[&#34;255.255.255.0&#34;,&#34;255.255.0.0&#34;,&#34;255.0.0.0&#34;],[&#34;255.255.255.0&#34;,&#34;255.255.0.0&#34;,&#34;255.0.0.0&#34;]]" />
	
		</div></div>
		
<div class="cbi-value" id="cbi-network-lan-gateway" data-index="6" data-depends="[{&#34;cbid.network.lan.proto&#34;:&#34;static&#34;}]"><label class="cbi-value-title" for="cbid.network.lan.gateway">IPv4 gateway</label>
		<div class="cbi-value-field">

	<input data-update="change" id="cbid.network.lan.gateway" name="cbid.network.lan.gateway" type="text" class="cbi-input-text" value="" data-type="ip4addr" data-optional="false" />
	
		</div></div>
		
			<h2 name="content">Join Network: Wireless Scan</h2>		
<div class="cbi-map">
	<fieldset class="cbi-section">
		<table class="cbi-section-table" style="empty-cells:hide;" id="wifiscantable">
		
<tr id="wifinetwork_CollectingInit" class="cbi-section-table-row cbi-rowstyle-1" style="vertical-align:middle; text-align:center; padding:3px;"><td class="cbi-value-field" style="width:16px; padding:3px">

<img src="img/signal-none.png"></td>
<td colspan="2" class="cbi-value-field" style="vertical-align:middle; text-align:center; padding:3px;">
<em>...</em></td>
</tr>				
		</table>
		
		<!-- footer -->
<table id="domoticzscan" class="cbi-section-table" style="empty-cells:hide;" >
<tr class="cbi-section-table-row cbi-rowstyle-1"><td class="cbi-value-field" style="width:16px; padding:3px"></td>
<td class="cbi-value-field" style="vertical-align:middle; text-align:left; padding:3px"></td>
<td style="width:40px"><input class="cbi-button cbi-button-find" value="Rescan" onclick="processAjaxRequest('wifiscantable', '/listwifinetworks',processAjaxWifiNetworks); return false;" type="submit"></td>
</tr>				
</table>


<!-- footer -->

		
	</fieldset>
</div>


</div>

<!-- proto_static end -->

