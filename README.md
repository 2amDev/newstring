Basic String Class Implimentaton 
====================

Simple c++ class, I made to get more experience with c++ constructors and operators,
intentionally utilizing only malloc and free.

Accepts and has converts between both c-style and wide char strings, as I have commonly found myself using or receiving a combination of the two,
yet never needing beyond the ASCII charset.

<pre><code>
wchar_t* w_str() //  get wide string
char* c_str() //  get c string
size_t length() // get string size ( excluding null terminator )
void append(T string) // append a string
bool equal(T string) // check if string is equal to another
bool contains(T string) // check if string contains a substring
</code></pre>


