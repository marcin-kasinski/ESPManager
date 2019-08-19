<h2><a id="content" name="content">Status</a></h2>



<fieldset class="cbi-section">

	<fieldset class="cbi-section">
		<legend>Backup / Restore</legend>
		<div class="cbi-section-descr">Click &#34;Generate archive&#34; to download a tar archive of the current configuration files. To reset the firmware to its initial state, click &#34;Perform reset&#34; .</div>
		<div class="cbi-section-node">
			<form class="inline" method="post" action="/getConfFile" name="getConfFile">
				<input type="hidden" name="token" value="">
				<div class="cbi-value">
					<label class="cbi-value-title" for="image">Download backup:</label>
					<div class="cbi-value-field">
						<input class="cbi-button cbi-button-apply" type="submit" name="backup" value="Generate archive" />
					</div>
				</div>
			</form>
			
			<form class="inline" method="post" action="/factoryReset" name="factoryReset">
				<input type="hidden" name="token"/>
				<div class="cbi-value cbi-value-last">
					<label class="cbi-value-title">Reset to defaults:</label>
					<div class="cbi-value-field">
						<input onclick="return confirm('Really reset all changes?')" class="cbi-button cbi-button-reset" type="submit" name="reset" value="Perform reset" />
					</div>
				</div>
			</form>
			
		</div>
		<br />
		<div class="cbi-section-descr">To restore configuration files, you can upload a previously generated backup archive here.</div>
		<div class="cbi-section-node">
			<form class="inline" method="post" action="/confupload" name="confupload"  enctype="multipart/form-data">
				<input type="hidden" name="token" value="confupload"/>
				<div class="cbi-value cbi-value-last">
					<label class="cbi-value-title" for="archive">Restore backup:</label>
					<div class="cbi-value-field">
						<input type="file" name="archive" id="archive" />
						<input type="submit" class="cbi-button cbi-input-apply" name="restore" value="Upload new conf..." />
					</div>
				</div>
			</form>
		</div>
	</fieldset>

	<br />

	<fieldset class="cbi-section">
		<legend>Flash new firmware image</legend>
		
			<form method="post" name="firmware" action="/firmware" enctype="multipart/form-data">
				<input type="hidden" name="token" id="token"/>
				<div class="cbi-section-descr">Upload a sysupgrade-compatible image here to replace the running firmware.</div>
				<div class="cbi-section-node">

					<div class="cbi-value cbi-value-last">
						<label class="cbi-value-title" for="image">Image:</label>
						<div class="cbi-value-field">
							<input type="file" name="update" id="image" />
							<input type="submit" class="cbi-button cbi-input-apply" value="Flash image..."/>
						</div>
					</div>
				</div>
				
			</form>
		
	</fieldset>

	
		<fieldset class="cbi-section">
		<legend>Flash new SPIFFS</legend>
		
			<form method="post" action="/firmwareSPIFFS" id="firmwareSPIFFS" name="firmwareSPIFFS" enctype="multipart/form-data">
				<input type="hidden" name="token"/>
				<div class="cbi-section-descr">Upload a sysupgrade-compatible image here to replace the running firmware.</div>
				<div class="cbi-section-node">
					
					<div class="cbi-value cbi-value-last">
						<label class="cbi-value-title" for="image">Image:</label>
						<div class="cbi-value-field">
							<input type="file" name="update" id="image" />
							<input type="submit" class="cbi-button cbi-input-apply" value="Flash SPIFFS..."/>
						</div>
					</div>
				</div>
				
			</form>
		
	</fieldset>

	
</fieldset>



	<script type="text/javascript">
	
function divReload()
{

updateRuntimeValues();


updateToken("getConfFile");
updateToken("factoryReset");
updateToken("confupload");
updateToken("firmware");
updateToken("firmwareSPIFFS");

	
}

document.addEventListener('DOMContentLoaded', function() {

//    alert("Ready! start");
	divReload();

	
}, false);
	
	
	</script>
