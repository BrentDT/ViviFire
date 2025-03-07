const chm = (location.protocol == "mk:");
const web = (location.protocol.indexOf("http") == 0);
if (web) setFavIcon('/ViviFire/favicon.ico');
const stub = (document.getElementById('stub') !== null);

const warning = document.createElement('p');
warning.className = 'nav';
warning.innerHTML =
	`This document ${stub?'is a stub':'may be incomplete'}.
	You can <a href="https://www.b6sw.com/ViviFire/docs/">see the recent changes here</a>.
	You can also give anonymous feedback. See the poll below the article.`;
document.body.insertBefore(warning, document.getElementsByClassName('breadcrumb')[0]);

if (!chm) {
	const contents = document.createElement('P');
	contents.innerHTML =
		`<a href="contents.html">Contents</a>
		| <a href="#" onclick="showSearch(this)">Search</a>`;
	document.body.insertBefore(contents, warning);
}

document.write(`<hr>
	<div id="poll" role="status" aria-live="polite" aria-relevant="text">
	<div role="form" aria-label="poll">
	<p>What is your rating for this article? (1 = lowest)</p>
	<button onclick="vote(0)">1</button>
	<button onclick="vote(1)">2</button>
	<button onclick="vote(2)">3</button>
	<button onclick="vote(3)">4</button>
	<p>or <a href="https://www.b6sw.com/forum/posting.php?mode=annotate&f=37&anchor=${/\/(\w+)\.html/.exec(location.href)[1]}" target="_blank">send a message</a></p>
	</div></div>`);

document.write(`<footer aria-label="footer">
	<p><a rel="license" href="http://creativecommons.org/publicdomain/mark/1.0/">
	<img src="cc0-80x15.png" style="border-style:none" alt="Public Domain Mark" />
	</a> This work is free of known copyright restrictions.</p>`);

if (!chm && document.lastModified) document.write(lastModifiedOrReviewed());
document.write('</footer>');

// Add 'Help' to the heading 'Syntax'.
var syntax = document.querySelector('h2+div.language-vivifire.syntax')?.previousElementSibling;
if (syntax) {
	syntax.style.position = 'relative';
	syntax.innerHTML +=
		'<span style="display: inline-block; position: absolute; right: 20px; font-size: 1rem; font-wait: normal;">[<a href="Typographic_Conventions.html" target="_blank">Help</a>]</span>';
}

function getFileName(uri) {
	return /[\/\\](\w+)\.html/.exec(uri)[1];
}

function vote(value) {
	const dark = (window.matchMedia && window.matchMedia('(prefers-color-scheme: dark)').matches) ? 1 : 0;
	const xmlhttp = (window.XMLHttpRequest) ? new XMLHttpRequest() : new ActiveXObject("Microsoft.XMLHTTP");
	xmlhttp.onreadystatechange = function() {
		if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
			document.getElementById("poll").innerHTML = xmlhttp.responseText;
		}
	};
	var article = getFileName(document.URL);
	xmlhttp.open('GET', `https://www.b6sw.com/ViviFire/poll/vote.php?article=${article}&vote=${value}&dark=${dark}`, true);
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
		`<form role="search" action="search.html" method="get" style="display: inline-block">
		<input type="search" id="q" name="q" required value="" placeholder="Search the manual">
		<button>Search</button>
		</form>`;
	document.getElementById('q').focus();
}

function lastModifiedOrReviewed() {
	var metaContent = document.getElementsByTagName('meta')['DC.date.reviewed']?.content || '1970';
	var lastReviewed = new Date(metaContent);
	var lastModified = new Date(document.lastModified);
	var verb = (Math.abs(lastModified - lastReviewed) < 1.5 * 86400000) ? 'reviewd ' : 'modified ';
	return '<p>Last '+verb+lastModified.toLocaleDateString()+'.</p>';
}
