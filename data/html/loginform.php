<form method="post" action="/login">
	<div class="cbi-map">
		<h2><a id="content" name="content">Authorization Required</a></h2>
		<div class="cbi-map-descr">
			Please enter your username and password.</div>
		<fieldset class="cbi-section"><fieldset class="cbi-section-node">
			<div class="cbi-value">
				<label class="cbi-value-title">Username</label>
				<div class="cbi-value-field">
					<input class="cbi-input-user" name="USERNAME" value="" type="text">
				</div>
			</div>
			<div class="cbi-value cbi-value-last">
				<label class="cbi-value-title">Password</label>
				<div class="cbi-value-field">
					<input class="cbi-input-password" name="PASSWORD" type="password">
				</div>
			</div>
		</fieldset></fieldset>
	</div>

	<div>
		<input value="Login" class="cbi-button cbi-button-apply" type="submit">
		<input value="Reset" class="cbi-button cbi-button-reset" type="reset">
	</div>
</form>


<script>

var token='X';
			
function divReload()
{
updateRuntimeValues();


}

document.addEventListener('DOMContentLoaded', function() {

	divReload();
	
}, false);
	
</script>

