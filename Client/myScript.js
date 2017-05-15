function init(){
    var i;
    var ctn=document.getElementsByName("content");
    var content=new Array(ctn.length);
    var a=new Array(ctn.length);
    var span=new Array(ctn.length);
    for(i=0;i<ctn.length;i++){  
        content[i]=ctn[i].innerHTML;        
        span[i]=document.createElement("span");
       	a[i]=document.createElement("a");
		span[i].innerHTML=content[i];
		a[i].setAttribute("i",i);
        a[i].innerHTML="-</br>";
        a[i].href="javascript:void(0)";
        a[i].onclick=function(){
		i=this.getAttribute("i");
        	if(a[i].innerHTML.indexOf("+")>=0){
            	a[i].innerHTML="-</br>";
                span[i].style.display="block";
           	}
            else{
            	a[i].innerHTML="+</br>";
                span[i].style.display="none";
            }
        }
        ctn[i].innerHTML="";
        ctn[i].appendChild(a[i]);
	ctn[i].appendChild(span[i]);
    }
}