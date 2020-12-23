var chm = (location.protocol == "mk:");
var web = (location.protocol.indexOf("http") == 0);
if (web) setFavIcon('/ViviFire/favicon.ico');
var stub = (document.getElementById('stub') !== null);

var warning = document.createElement('p');
warning.className = 'nav';
warning.innerHTML = 'This document '+(stub?'is a stub':'may be incomplete')+'.'
	+' You can <a href="https://www.b6sw.com/ViviFire/docs/">see the recent changes here</a>.'
	+' You can also give anonymous feedback. See the poll below the article.'
	+' NOTE: We are in the middle of a change to <a href="https://en.wikipedia.org/wiki/Simplified_Technical_English" target="_blank">Simplified Technical English</a>.';
document.body.insertBefore(warning, document.body.firstChild);

if (!chm) {
	var contents = document.createElement('P');
	contents.innerHTML = '<a href="contents.html">Contents</a> | <a href="https://www.b6sw.com/forum/posting.php?mode=annotate&f=37&anchor='+/\/(\w+)\.html/.exec(location.href)[1]+'" target="_blank">Send feedback</a>';
	document.body.insertBefore(contents, document.body.firstChild);
}

///var bc = document.getElementsByClassName('breadcrumb'); if (bc) bc[0].setAttribute('aria-label', 'Breadcrumb');

document.write('<hr><p>'+
'<div id="poll" role="status" aria-live="polite" aria-relevant="text">' +
'<form>' +
'This article is' +
' <input id="0" type="radio" onclick="vote(0)"><label for="0">good</label>' +
' <input id="1" type="radio" onclick="vote(1)"><label for="1">bad</label>' +
' <input id="2" type="radio" onclick="vote(2)"><label for="2">messy</label>' +
' <input id="3" type="radio" onclick="vote(3)"><label for="3">confusing</label>' +
' <input id="4" type="radio" onclick="vote(4)"><label for="4">inaccurate</label>' +
'</form>' +
'</div></p>');

if (!chm && document.lastModified) document.write('<footer><p>Last modified '+document.lastModified+'.</p></footer>');

function getFileName(uri) {
	return /[\/\\](\w+)\.html/.exec(uri)[1];
}

function vote(value) {
	var xmlhttp;
	if (window.XMLHttpRequest)
		xmlhttp = new XMLHttpRequest();
	else
		xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
	xmlhttp.onreadystatechange = function() {
		//alert('readyState='+xmlhttp.readyState+' && status='+xmlhttp.status+' && text='+xmlhttp.responseText);
		if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
			document.getElementById("poll").innerHTML = xmlhttp.responseText;
		}
	};
	var article = getFileName(document.URL);
	xmlhttp.open('GET', 'https://www.b6sw.com/ViviFire/poll/vote.php?article='+article+'&vote='+value, true);
	xmlhttp.send();
}

function setFavIcon(url) {
	var head = document.getElementsByTagName('head')[0];
	var link = document.createElement('link');
	link.type = 'image/x-icon';
	link.rel = 'icon';
	link.href = url;
	head.appendChild(link);
}
