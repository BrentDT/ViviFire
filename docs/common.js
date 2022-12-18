var chm = (location.protocol == "mk:");
var web = (location.protocol.indexOf("http") == 0);
if (web) setFavIcon('/ViviFire/favicon.ico');
var stub = (document.getElementById('stub') !== null);

var warning = document.createElement('p');
warning.className = 'nav';
warning.innerHTML = 'This document '+(stub?'is a stub':'may be incomplete')+'.'
	+' You can <a href="https://www.b6sw.com/ViviFire/docs/">see the recent changes here</a>.'
	+' You can also give anonymous feedback. See the poll below the article.';
document.body.insertBefore(warning, document.body.firstChild);

if (!chm) {
	var contents = document.createElement('P');
	contents.innerHTML = 
		'<a href="contents.html">Contents</a>'+
		' | <a href="#" onclick="showSearch(this)">Search</a>'+
		' | <a href="https://www.b6sw.com/forum/posting.php?mode=annotate&f=37&anchor='+/\/(\w+)\.html/.exec(location.href)[1]+'" target="_blank">Send feedback</a>';
	document.body.insertBefore(contents, document.body.firstChild);
}

///var bc = document.getElementsByClassName('breadcrumb'); if (bc) bc[0].setAttribute('aria-label', 'Breadcrumb');

document.write('<hr><p>'+
'<div id="poll" role="status" aria-live="polite" aria-relevant="text">' +
'<div role="form" aria-label="poll">' +
'<p>What do you think about this article?</p>' +
'<button onclick="vote(0)">Good</button>' +
'<button onclick="vote(1)">Bad</button>' +
'<button class="touch-only" onclick="vote(2)">Bad on mobile</button>' +
'</div>' +
'</div></p>');

document.write('<footer aria-label="footer">'+
'<p>'+
'<a rel="license" href="http://creativecommons.org/publicdomain/mark/1.0/">'+
'<img src="cc0-80x15.png" style="border-style:none" alt="Public Domain Mark" />'+
'</a> This work is free of known copyright restrictions.'+
'</p>');
if (!chm && document.lastModified) document.write(lastModifiedOrReviewed()); //'<p>Last modified '+document.lastModified+'.</p>');
document.write('</footer>');

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

function showSearch(elem) {
	elem.outerHTML =
		'<form role="search" action="search.html" method="get" style="display: inline-block">'+
		'<input type="search" id="q" name="q" required value="" placeholder="Search the manual">'+
		'<button>Search</button>'+
		'</form>';
	document.getElementById('q').focus();
}

function lastModifiedOrReviewed() {
	var metaContent = document.getElementsByTagName('meta')['DC.date.reviewed']?.content || '1970';
	var lastReviewed = new Date(metaContent);
	var lastModified = new Date(document.lastModified);
	var verb = (Math.abs(lastModified - lastReviewed) < 1.5 * 86400000) ? 'reviewd ' : 'modified ';
	return '<p>Last '+verb+lastModified.toLocaleDateString()+'.</p>';
}