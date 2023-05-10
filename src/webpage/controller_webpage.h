const char MAIN_PAGE[] PROGMEM = R"HTMLPAGE(
<!DOCTYPE html>
<html  lang="es" xml:lang="es" xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<title>NIOT Domotica Controller</title>
	</head>
	<style>
		* {
			margin: 0px;
			padding: 0px;
		}

		body {
			background-color: rgb(196, 196, 196);
		}

		.navbar {
			width: 100%;
			margin: 0;
			background-color: #FFF;
			color: #000000;
			border-bottom: 5px solid #5e5e5e;
		}

		.titulo {
			font-size: calc(50px + 0.390625vw);
			font-family: 'Arial Black';
			color: #414141;
		}

		main {
			width: 100%;
		}

		.section-title {
			font-size: calc(35px + 0.390625vw);
			font-weight: bold;
			font-family: 'Arial Black';
			color: #414141;
			line-height: 50px;
			padding: 20px 10px 0px 10px;
			color: #000000;
		}

		td {
			background: rgb(230, 230, 230);
		}

		.table-borders {
			overflow: hidden;
			border-radius: 1em !important;
		}

		.table-margin {
			margin-left: 10px;
			margin-right: 10px;
		}

		.table-title-row {
			width: 50%;
		}

		.table-text {
			font-family: 'Arial Black';
			color: #000000;
		}

		.table-title-text {
			font-size: calc(21px + 0.390625vw);
			margin-left: 10px;
		}

		.table-data-text {
			font-size: calc(21px + 0.390625vw);
			font-family: 'Arial';
			margin-right: 10px;
			text-align: center;
		}

		.table-switch-style {
			height: calc(50px + 0.390625vw);
		}

		.table-data-style {
			height: calc(50px + 0.390625vw);
		}

		.table-range-style {
			height: calc(50px + 0.390625vw);
			color:#5e5e5e;
			display: flex;
		}

		.table-range-align {
			width: 100%;
			align-self: center;
		}

		.noselect {
			-webkit-touch-callout: none;
			-webkit-user-select: none;
			-khtml-user-select: none;
			-moz-user-select: none;
			-ms-user-select: none;
			user-select: none;
		}

		.range-input {
			align-self: center;
		}
	</style>

	<body onload="process()">
		<header>
			<div class="navbar">
				<h1 class="titulo">Panel de control</h1>
			</div>
		</header>
		<main>
			<div class="section-title">Sensores</div>
			<table class="table-margin table-borders table-text" style="width:calc(100% - 20px)">
				<colgroup>
					<col span="1" class="table-title-row">
					<col span="1" class="table-title-row">
				</colgroup>
				<col span="2" style="background-color:rgb(0,0,0); color:#FFFFFF">
				<col span="2" style="background-color:rgb(0,0,0); color:#FFFFFF">
				<tr class="table-data-style">
					<td><div class="table-title-text">Temperatura</div></td>
					<td><div class="table-data-text" id="temp0">1</div></td>
				</tr>
				<tr class="table-data-style">
					<td><div class="table-title-text">Humedad</div></td>
					<td><div class="table-data-text" id="hum0">2</div></td>
				</tr>
			</table>
			<div class="section-title">Luces</div>
			<table class="table-margin table-borders table-text" style="width:calc(100% - 20px)">
				<colgroup>
					<col span="1" class="table-title-row">
					<col span="1" class="table-title-row">
				</colgroup>
				<col span="2" style="background-color:rgb(0,0,0); color:#FFFFFF">
				<col span="2" style="background-color:rgb(0,0,0); color:#FFFFFF">
				<tr >
					<td><div class="table-title-text">Sala</div></td>
					<td class="table-switch-style noselect"><div class="table-data-text" id="light0" onclick="changeLightState(this)">OFF</div></td>
				</tr>
				<tr>
					<td><div class="table-title-text">Comedor</div></td>
					<td class="table-switch-style noselect"><div class="table-data-text" id="light1" onclick="changeLightState(this)">OFF</div></td>
				</tr>
			</table>
			<div class="section-title">Persianas</div>
			<table class="table-margin table-borders table-text" style="width:calc(100% - 20px)">
				<colgroup>
					<col span="1" class="table-title-row">
					<col span="1" class="table-title-row">
				</colgroup>
				<col span="2" style="background-color:rgb(0,0,0); color:#FFFFFF">
				<col span="2" style="background-color:rgb(0,0,0); color:#FFFFFF">
				<tr >
					<td><div class="table-title-text">Sala</div></td>
					<td class="noselect">
						<div class="table-range-style table-data-text">
							<div class="table-range-align">
								<label for="louver0">Apertura</label>
								<input
								type="range"
								class="range-input"
								id="louver0"
								onfocus="this.oldvalue = this.value;"
								onchange="changeLouverState(this);this.oldvalue = this.value;"
								min="0"
								max="180">
							</div>
						</div>
					</td>
				</tr>
				<tr>
					<td><div class="table-title-text">Comedor</div></td>
					<td class="noselect">
						<div class="table-range-style table-data-text">
							<div class="table-range-align">
								<label for="louver1">Apertura</label>
								<input type="range"
								class="range-input"
								id="louver1"
								onfocus="this.oldvalue = this.value;"
								onchange="changeLouverState(this);this.oldvalue = this.value;"
								min="0"
								max="180">
							</div>
						</div>
					</td>
				</tr>
			</table>
		</main>
	</body>
	<script type="text/javascript">
    var tempTags = ["temp0"];
    var humTags = ["hum0"];
    var lightTags = ["light0", "light1"];
    var louverTags = ["louver0", "louver1"];
	var switchColorVal = {
		"OFF": "#780b00",
		"ON": "green"
	}
	var xmlHttp = createXmlHttpObject();
    var interval = setInterval(process, 2500);

    function setSwitchColours() {
		const switches = document.querySelectorAll(".table-switch-style");
		for (const thisSwitch of switches) {
			thisSwitch.style.color = "white";
			thisSwitch.style.background = switchColorVal[thisSwitch.outerText];
		}
    }

    function process() {
		xmlHttp = new XMLHttpRequest();
		if(xmlHttp.readyState == 0 || xmlHttp.readyState == 4) {
			xmlHttp.open("PUT","xml", true);
			xmlHttp.onreadystatechange = manageResponse;
			xmlHttp.send(null);
		}
		setTimeout("manageResponse()", 200);
    }

    function manageResponse() {
		var xmlResponse = xmlHttp.responseXML;
		if (xmlResponse == null) {
			return;
		}
		setTemperatures(xmlResponse);
		setHumidities(xmlResponse);
		setLightSwitches(xmlResponse);
		setLouvers(xmlResponse);
    }

    function setTemperatures(xmlResponse) {
		for (const thisTempTag of tempTags) {
			var tempElement = document.getElementById(thisTempTag);
			var xmlDoc =  xmlResponse.getElementsByTagName(thisTempTag);
			tempElement.innerHTML = xmlDoc[0].firstChild.nodeValue + '&deg;C';
		}
    }

    function setHumidities(xmlResponse) {
		for (const thisHumTag of humTags) {
			var humElement = document.getElementById(thisHumTag);
			var xmlDoc = xmlResponse.getElementsByTagName(thisHumTag);
			humElement.innerHTML = xmlDoc[0].firstChild.nodeValue + " %";
		}
    }

    function setLightSwitches(xmlResponse) {
		for (const thisLightTag of lightTags) {
			var lightElement = document.getElementById(thisLightTag);
			var xmlDoc = xmlResponse.getElementsByTagName(thisLightTag);
			lightElement.innerHTML = xmlDoc[0].firstChild.nodeValue;
			lightElement.style.background = switchColorVal[lightElement.outerText];
		}
		setSwitchColours();
    }

    function setLouvers(xmlResponse) {
		for (const thisLouverTag of louverTags) {
			var louverElement = document.getElementById(thisLouverTag);
			var xmlDoc = xmlResponse.getElementsByTagName(thisLouverTag);
			louverElement.value = xmlDoc[0].firstChild.nodeValue;
		}
    }

	function createXmlHttpObject() {
		if (window.XMLHttpRequest) {
			xmlHttp = new XMLHttpRequest();
			return;
		}
		xmlHttp =  new ActiveXObject("Microsoft.XMLHTTP");
	}

	function changeLightState(element) {
		var xhttp = new XMLHttpRequest();
		var newValue = null;
		if (element.innerHTML == "ON") {
			newValue = "OFF"
		} else {
			newValue = "ON"
		}
		xhttp.onreadystatechange = function() {
			if (this.readyState == 4 && this.status == 200) {
				element.innerHTML = newValue;
				element.style.background = switchColorVal[element.innerHTML];
				element.parentNode.style.background = switchColorVal[element.innerHTML];
			}
		}
		xhttp.open("PUT", "updateLightState?" +
			"lightId=" + element.id +
			"&lightState=" + newValue, true);
		xhttp.send();
	}

	function changeLouverState(element) {
		var xhttp = new XMLHttpRequest();
		var newValue = element.value;
		var oldValue = element.oldvalue;
		xhttp.onreadystatechange = function() {
			if (this.readyState == 4 && this.status == 200) {
				element.value = newValue;
			} else {
				element.value = oldValue;
			}
		}
		xhttp.open("PUT", "updateLouverState?" +
			"louverId=" + element.id +
			"&louverAngle=" + element.value, true);
		xhttp.send();
	}
	</script>
</html>
)HTMLPAGE";
