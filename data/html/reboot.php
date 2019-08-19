

<script type="text/javascript">//<![CDATA[

function getDatetime()
{
		var d = new Date();var n = d.getTime();
		return n;
}

function setLocation(location)
{
		window.location=location;
}


function rebootTimer()
{
		setLocation('/');

}

	function processAjaxReboot(divid, file, injson)  
	{

		setElementVisibility('rebootButton',0);
		setElementVisibility('reboot-message',1,'');
		setTimeout(rebootTimer, 30000);
	
	}
		
//]]></script>

<h2 name="content">Reboot</h2>
<br />

<p>Reboots the operating system of your device</p><hr />


<input class="cbi-button cbi-button-apply" type="button" id="rebootButton" value="Perform reboot" onclick="processAjaxRequest('', '/reboot?token='+token+'&amp;datetime='+getDatetime(),processAjaxReboot);" />

<p class="alert-message" style="display:none" id="reboot-message">
	<img src="/img/loading.gif" alt="Loading" style="vertical-align:middle" />
	<span>Device is rebooting...</span>
</p>

<script>
			
function divReload()
{
updateRuntimeValues();


}

document.addEventListener('DOMContentLoaded', function() {

	divReload();
	
}, false);
	
</script>

