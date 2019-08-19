<!DOCTYPE html>
<html lang="auto">
 <head>
  <meta charset="utf-8">
  <title id="htmltitle" ></title>
  <!--[if lt IE 9]><script src="/luci-static/bootstrap/html5.js"></script><![endif]-->
  <meta name="viewport" content="initial-scale=1.0">
  <link rel="stylesheet" href="cascade.css">
<!--  <link rel="stylesheet" media="only screen and (max-device-width: 854px)" href="mobile.css" type="text/css" />
  <link rel="shortcut icon" href="img/favicon.ico">
  <script src="xhr.js"></script>-->
 </head>

 <body class="lang_autoOverview">
  <header>
   <div class="fill">
    <div class="container">
     <a id="devicetitle" class="brand" href="#">ESP Device</a>
     <ul class="nav">

	 
        <li class="dropdown">
            <a class="menu" href="#">System</a>
	 <ul class="dropdown-menu">
	  <li><a href="/">System Info</a></li>
	  <li><a href="/conf">Administration</a></li>
	  <li><a href="/tasks">Scheduled Tasks</a></li>
	  <li><a href="/firmware">Backup / Flash Firmware</a></li>
	  <li><a href="/logs">System Log</a></li>
	  <li><a id="reboot" onClick="var d = new Date();var n = d.getTime();this.href =this.href+'?tok='+n+'&amp;token='+token ;" href="/rebootask">Reboot</a></li>
	</ul>
        </li>
		
         <li>
	        <a href="/">Logout</a>
        </li> 
    
	</ul>

	<div class="pull-right">
	
      <span id="xhr_poll_status" >
       <span class="label success">Firmware version:</span><span class="label success" id="firmware_version"></span>
      </span>
     </div>
	
    </div>
   </div>
  </header><div id="maincontent" class="container">

<script>

function updateToken(formname)
{
	var oForm = document.forms[formname];
	var otoken = oForm.elements["token"];
//	alert('1 '+otoken.value);
	otoken.value=token;
//	alert('2 '+otoken.value);

}	
				
function addOptionToSelect(key, value, text) 
{
	var selectelement = document.getElementById(key);
		 
	var option= document.createElement( 'option' );
	option.value = value;
	option.text = text;
	selectelement.add( option );
	
}

function processAjaxSwitchChange(divid, file, injson) 
{
	var obj = JSON.parse(injson);
											
	var size = obj.runtime.length; 
	
				for (i = 0; i < size; i++) 
				{
					updateRuntimeValue(obj.runtime[i].n.trim(),obj.runtime[i].v.trim());
				}

processRelayState(0);
processRelayState(1);
//obsluga led
processRGBLedState();
	
}			


function processAjaxRequest(divid, file, callbackfunction, async) 

{

 if (typeof(async)=='undefined') async=true;


  var xmlhttp = new XMLHttpRequest();

    xmlhttp.onreadystatechange = function() {
        if (xmlhttp.readyState == XMLHttpRequest.DONE ) {
           if (xmlhttp.status == 200) {

			callbackfunction(divid, file, xmlhttp.responseText);
			
           }
           else if (xmlhttp.status == 400) {
              alert('There was an error 400');
           }
           else {
               alert('something else other than 200 was returned');
           }
        }
    };

    xmlhttp.open("GET", file, async);
    xmlhttp.send();
			
}			

var runtimevalues;

function updateRuntimeValuesGlobalVariables(inruntimevalues)
{
//	alert ('updateRuntimeValuesGlobalVariables ');
	runtimevalues=inruntimevalues;
}


function MQTTfieldsSetVisibility(visibility)
{
//	alert ('MQTTfieldsSetVisibility ');

	setElementAttribute('mqtt.hostname' ,'data-optional', !visibility) ;
	setElementAttribute('mqtt.domoticzhostname' ,'data-optional', !visibility) ;
	setElementAttribute('mqtt.domoticzport' ,'data-optional', !visibility) ;
	setElementAttribute('mqtt.domoticzusername' ,'data-optional', !visibility) ;
	setElementAttribute('mqtt.domoticzuserpassword' ,'data-optional', !visibility) ;
	setElementAttribute('mqtt.domoticzidx' ,'data-optional', !visibility) ;
	setElementVisibility('cbi-network-mqtt-hostname',visibility);
	setElementVisibility('cbi-network-mqtt-domoticzhostname',visibility);
	setElementVisibility('cbi-network-mqtt-domoticzport',visibility);
	setElementVisibility('cbi-network-mqtt-domoticzusername',visibility);
	setElementVisibility('cbi-network-mqtt-domoticzuserpassword',visibility);
	setElementVisibility('cbi-network-mqtt-domoticzidx',visibility);
		
}

function updateRuntimeValue(key,value)
{
//	alert ('updateRuntimeValue ');
	
			var size = runtimevalues.length; 

//	alert ('updateRuntimeValue size '+size);
			
			for (i = 0; i < size; i++) {

			
				var Namestr = String(runtimevalues[i].n.trim());
				if (Namestr.localeCompare(key)==0){runtimevalues[i].v=value;break};
								
			}
			
}

function getRuntimeValueByKey(key)
{

//			if (key=='mqtt.domoticzhostname') alert('getRuntimeValueByKey');


var ret=null;
			var size = runtimevalues.length; 
//			if (key=='mqtt.domoticzhostname') alert('size '+size);
			
			var i;
			
			for (i = 0; i < size; i++) {

			
								var Namestr = String(runtimevalues[i].n.trim());
								
								if (Namestr.localeCompare(key)==0){ret= runtimevalues[i].v.trim();break};
								
			}

//			if (key=='mqtt.domoticzhostname') alert('ret '+ret);
			
			return ret;
			
}

function RelayfieldsSetVisibility(index,visibility)
{

//alert('index '+index+' , visibility '+visibility);

if (visibility<0) visibility=0;
else visibility=1;

	if (visibility==0) 
	{	

//	alert('visibility==0');


		setValue('relay'+index+'.relay_switch_pin' ,-1) ;
		setValue('relay'+index+'.relay_led_pin' ,-1) ;

//			alert('... '+		getValue('relay'+index+'.relay_led_pin' ) );

		
//		setElementAttribute('relay'+index+'.relay_switch_pin' ,'data-optional', !visibility) ;
//		setElementAttribute('relay'+index+'.relay_led_pin' ,'data-optional', !visibility) ;

	}

	setElementAttribute('relay'+index+'.internalName' ,'data-optional', !visibility) ;
	setElementVisibility('cbi-relay'+index+'-internalName',visibility);

	setElementAttribute('relay'+index+'.relay_conn_type' ,'data-optional', !visibility) ;
	setElementVisibility('cbi-relay'+index+'-relay_conn_type',visibility);

	setElementAttribute('relay'+index+'.relay_switch_pin' ,'data-optional', !visibility) ;
	setElementVisibility('cbi-relay'+index+'-relay_switch_pin',visibility);
	
	setElementAttribute('relay'+index+'.relay_switch_type' ,'data-optional', !visibility) ;
	setElementVisibility('cbi-relay'+index+'-relay_switch_type',visibility);
	
	setElementAttribute('relay'+index+'.domoticzidx' ,'data-optional', !visibility) ;
	setElementVisibility('cbi-relay'+index+'-domoticzidx',visibility);
	
	setElementVisibility('cbi-relay'+index+'-relay_switch',visibility);

	setElementAttribute('relay'+index+'.relay_led_pin' ,'data-optional', !visibility) ;
	setElementVisibility('cbi-relay'+index+'-relay_led_pin',visibility);

	setElementAttribute('relay'+index+'.enablealexa' ,'data-optional', !visibility) ;
	setElementVisibility('cbi-relay'+index+'-enablealexa',visibility);

	var val  = getValue('relay'+index+'.relay_led_pin');

		if (val==null)return ;
	
	if (val<0 || visibility==0 )visibility=0;
else visibility=1;

	
	setElementAttribute('relay'+index+'.relay_led_conn_type' ,'data-optional', !visibility) ;
	setElementVisibility('cbi-relay'+index+'-relay_led_conn_type',visibility);

//				cbi_init();

//			alert('... END ');
	
}

function updateRuntimeValuesFromGlobalVariables()
{

			var size = runtimevalues.length; 

//			alert('updateRuntimeValuesFromGlobalVariables START size '+size );
			
//			alert(runtimevalues[size-1].name.trim() +' '+runtimevalues[size-1].value.trim()+' '+runtimevalues[size-1].h.trim());
			
			for (i = 0; i < size; i++) {

//								if (runtimevalues[i].n.trim() =='relay1.relay_pin') alert(runtimevalues[i].n.trim() +' / '+runtimevalues[i].v.trim());
			
								setValue(runtimevalues[i].n.trim() ,runtimevalues[i].v.trim(),runtimevalues[i].h.trim());
								
								var Namestr = String(runtimevalues[i].n.trim());
								
//								var valueInt=parseInt(runtimevalues[i].v.trim(),10);

								if (Namestr.localeCompare('mqtt.MQTT_enable')==0) MQTTfieldsSetVisibility(parseInt(runtimevalues[i].v.trim(),10));
								else if (Namestr.localeCompare('relay0.relay_pin')==0) RelayfieldsSetVisibility(0,parseInt(runtimevalues[i].v.trim(),10));
								else if (Namestr.localeCompare('relay1.relay_pin')==0) RelayfieldsSetVisibility(1,parseInt(runtimevalues[i].v.trim(),10));
								
			}

//			alert('updateRuntimeValuesFromGlobalVariables END1');

//			cbi_init();

//			alert('updateRuntimeValuesFromGlobalVariables END2');

			
}


function processAjaxupdateRuntimeValues(divid, file, injson) 
{

//	alert ('processAjaxupdateRuntimeValues 1 '+injson);

	 try {
			var obj = JSON.parse(injson);

} catch(e) {
        alert(e); // error in the above string (in this case, yes)!
    }			
			

//	alert ('processAjaxupdateRuntimeValues 2');
			
											
			updateRuntimeValuesGlobalVariables(obj.runtime);
			updateRuntimeValuesFromGlobalVariables();

}

function updateRuntimeValues()
{

//alert ('updateRuntimeValues ');
//	alert ('updateRuntimeValues ');

	
 if (typeof(runtimevalues)!='undefined') 
{
				updateRuntimeValuesFromGlobalVariables();
return;
}

processAjaxRequest('----', '/handleJSONRuntimeInfo?token='+token,processAjaxupdateRuntimeValues,false); 
												
}


function getElement(id ) {
var element=document.getElementById(id);
return 	element;
}

function removeElementAttribute(id, attribute ) {
var element=document.getElementById(id);
element.removeAttribute(attribute)

}

function setElementAttribute(id ,attribute, value) {

var element=document.getElementById(id);

if (element!=null) element.setAttribute(attribute,value);
}


function setElementVisibility(id ,value, showvalue) {
var element=document.getElementById(id);

if (typeof(showvalue)==='undefined') showvalue = 'display:table';

if (element==null) return;
if (value=="0" || value=="false" || value==false ) element.setAttribute('style','display:none');
if (value=="1" || value=="true"  || value==true ) element.setAttribute('style',showvalue);

}



function getValue(id ) {

var element=document.getElementById(id);
if (element==null) return null;
return element.value;

}

function setValue(id ,value, hidevalue) {

var tagname="";
var type="";
var element=document.getElementById(id);

if (element!=null) tagname=element.tagName;
if (element!=null && tagname=='INPUT') type=element.type;

if (value==hidevalue)
{
var elementlabel=document.getElementById(id+'label');
var elementbr=document.getElementById(id+'br');
	
if (elementlabel!=null)	elementlabel.style.display='none';
if (element!=null)	element.style.display='none';
if (elementbr!=null)	elementbr.style.display='none';

	
}


if (element==null) return;

if (tagname=='INPUT' && type == "checkbox" && (value=="0" || value=="false"  )  )  {value=="0";element.checked = false;}
else if (tagname=='INPUT' && type == "checkbox" && (value=="1" || value=="true"  )  )    {value=="1";element.checked = true;}
else if (tagname=='INPUT' || tagname=='SELECT')    element.value = value;

else if ('textContent' in element) {

element.textContent = value;

} 
else {
    element.innerText = value;
}

//if (id =='relay1.relay_pin') alert('XXX 2 '+element.value);
//if (id =='relay1.relay_pin') alert('XXX 3 '+getValue('relay1.relay_pin'));

//alert("setValue end " +id);

}

</script>

