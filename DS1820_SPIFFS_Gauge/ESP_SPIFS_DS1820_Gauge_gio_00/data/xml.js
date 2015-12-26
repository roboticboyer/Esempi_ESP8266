xmlHttp=new XMLHttpRequest();
function process(){
 if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
   xmlHttp.open('PUT','xml',true);
   xmlHttp.onreadystatechange=handleServerResponse; // no brackets?????
   xmlHttp.send(null);
 }
 setTimeout('process()',10000);
}
function handleServerResponse(){
 if(xmlHttp.readyState==4 && xmlHttp.status==200){
   xmlResponse=xmlHttp.responseXML;
   xmldoc = xmlResponse.getElementsByTagName('response');
   message = xmldoc[0].firstChild.nodeValue;
   //document.getElementById('runtime').innerHTML=message;
   Gauge.Collection.get('gauge1').setValue(message);
 }
}
