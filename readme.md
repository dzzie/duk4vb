
This project is a build of the C DukTape JavaScript engine
to be usable for visual basic 6.

Compiled size in release mode should about 400 K when statically
linked. 

The reason for this project, is that the Microsoft script control
is quite old at this point and does not support many of the new
JavaScript features. Even simple things like the following are 
unsupported:

str = 'test'; alert(str[1])

The Microsoft script control also does not support debugging,
which is a large weakness.

The core duktape engine is pretty much unchanged. I only did a 
couple small tweaks that are documented at the top of main.c
which is the wrapper to give VB access to its main api.

There are 3 vb6 test projects.

<pre>
basic: very lite weight just basic access to js engine

com:   allows the js to access arbitrary COM and vb6 host objects
       there are a few limitations but probably 96%+ coverage for normal needs.
       see \with_COM\COM\readme.txt

debugger: full GUI debugger. uses scintilla for IDE edit control supports 
          the debug protocol. see readme, and video demo below:
          https://www.youtube.com/watch?v=nSr1-OugQ1M
</pre>

 <img src="https://raw.githubusercontent.com/dzzie/duk4vb/master/vb_examples/with_debug/screenshot.png">
          
<pre>
benchmarks:
     * get the duktape javascript engine working with vb6
     * wire a general proxy between js and COM objects
     * create a COM bindings generator from vb6 prototypes
     * classify the engine and simplify methods to mimic ms script control
     * timeout mechanism and fatal app handler
     * auto loading of dependancies
     * handling of object return types (form.text1.text = etc)
     * integrate with debugger protocol of engine
     * use scintilla (scivb) control to provide optional IDE/debugger UI
</pre>

<pre>
Authors:
	* Duktape - <a href="DukTape_AUTHORS.rst">DukTape_AUTHORS.rst</a> - site: <a href="http://duktape.org">http://duktape.org</a>
	* Scintilla by Neil Hodgson [neilh@scintilla.org] - site: <a href="http://www.scintilla.org/">http://www.scintilla.org/</a>
	* ScintillaVB by Stu Collier - site: <a href="http://www.ceditmx.com/software/scintilla-vb/">http://www.ceditmx.com/software/scintilla-vb/</a>
	* CSubclass by Paul Canton [Paul_Caton@hotmail.com]
	* Interface by David Zimmer - site <a href="http://sandsprite.com">http://sandsprite.com</a>

</pre>
