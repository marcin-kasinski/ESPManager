
<h2><a id="content" name="content">System Log</a></h2>
<div id="content_syslog">
<textarea readonly="readonly" wrap="off" id="syslog"></textarea>
</div>

<script>

	

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

function processAjaxListLogs(divid, file, injson) 
{
		var obj = JSON.parse(injson);
											
		var size = obj.Logs.length; 
			
		var syslog=document.getElementById(divid);
	
			syslog.rows=size;
	
	    for (i = 0; i < size; i++) 
		{
	
		
		    var datetime=obj.Logs[i].datetime.trim();
//			alert ('datetime '+datetime);
			var message=obj.Logs[i].message.trim();
//			alert ('message '+message);

			syslog.value=syslog.value+datetime+' '+ message+'\n';

			
		}	//for
}			
			
document.addEventListener('DOMContentLoaded', function() {

updateRuntimeValues();

processAjaxRequest('syslog', '/handleJSONListLogs',processAjaxListLogs);			

}, false);
	

</script>
	
</form>