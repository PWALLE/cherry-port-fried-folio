<blockquote>
  <p><em>SampleCode Note</em>
  This is a compiler for a programming language called ‘Cool’,
  which I developed in tandem with a fellow grad student
  for my graduate-level Compilers course. The compiler
  is coded in Java, and leverages the Beaver parser generator
  and LLVM compiler infrastructure.</p>
</blockquote>

<h1>README</h1>

<p>@author: Paul Elliott and Monisha Balireddi <br>
@date: 6/11/13</p>

<h3>Notes</h3>

<p>Our version of the Cool Compiler, dubbed <em>[Not-As-]Cool Compiler</em>,
has been scaled down in some trivial ways.  For one,
case statements have been removed from the language.
Two, Symbols have been removed for now--this means that
the Symbol functionality of IO has also been removed.
Three, String functionality has been reduced--we decided
to represent Strings as a basic type i8* character pointer in LLVM,
and therefore we did not get a chance to create the specialty String functions.
Four, the toString functionality of the Any class has been removed.
Lastly, there is no ArrayAny class.</p>

<p>The current state of our compiler project:</p>

<ol>
<li>Scanning works as expected</li>
<li>Parsing works as expected</li>
<li>Generating the AST works as expected</li>
<li>Type checking works as expected</li>
<li>Code generation produces an llvm file called <code>main.ll</code>,
which unfortunately is broken.  The error seems to be
an insipid off-by-one somewhere, causing the type
names of a few operations to be mangled. We tried
debugging this to no avail prior to the deadline.</li>
<li>The script <code>run.sh</code> runs everything up to this point
and tries to compile <code>main.ll</code>. It breaks on the offending
instructions.  Most everything is correct, minus a few instructions.</li>
</ol>

<h3>Dependencies</h3>

<ol>
<li>Clang and LLC from LLVM (<a href="http://llvm.org/ llvm">here</a>)</li>
<li>Beaver (included in repo)</li>
</ol>

<h3>Contents</h3>

<p>This directory should contain:</p>

<ol>
<li><code>main/</code>
<ol>
<li><code>run.sh</code> - a shell script for compiling/running the compiler</li>
<li><code>scanner.flex</code> - a JFlex program containing the scanner definition</li>
<li><code>Driver.java</code> - a Driver program for testing the Cool parser/AST builder</li>
<li><code>ErrorReport.java</code> - an error reporting class</li>
<li><code>cool.grammar</code> - a Beaver grammar specification for a LALR(1) parser</li>
<li><code>Terminals.java</code> - a class containing the Terminal IDs</li>
<li><code>CodeGenerator.java</code> - the fancy shmancy code generation class</li>
</ol></li>
<li><code>beaver/</code>
<ol>
<li><code>Scanner.java</code> - part of Beaver scanner api</li>
<li><code>Symbol.java</code> - part of Beaver scanner api</li>
<li><code>Action.java</code> - part of Beaver api</li>
<li><code>Parser.java</code> - part of Beaver api</li>
<li><code>ParsingTables.java</code> - part of Beaver api</li>
<li><code>beaver-cc-0.9.11.jar</code> - jar file used to execute Beaver on a grammar file</li>
<li><code>beaver-cc.jar</code> - (older) jar file "" "" ""</li>
</ol></li>
<li><code>ast/</code>
[This directory contains all of the Java class files necessary
 for building the AST--including abstract syntax classes. 
 I will not enumerate everything here in the interest of brevity.]</li>
<li><code>stat/</code>
[This directory holds old copies of the .stat file, produced by Beaver.]</li>
<li><code>typecheck/</code>
<ol>
<li><code>Environment.java</code> - typecheck environment</li>
<li><code>HashStack.java</code> - data structure for type checking</li>
<li><code>TreeWalker.java</code> - modified AST Tree Walker now performs type checking</li>
</ol></li>
<li><code>README.md</code> - this file</li>
</ol>

<h3>How To Run</h3>

<p>Open <code>main/</code> directory on the command line.</p>

<h4>Configuration</h4>

<p>Open <code>run.sh</code> in a text editor and replace the line <code>JFLEX='jflex'</code> with your
local path to JFlex. For example, if JFlex is <code>/usr/share/weird_place/jflex</code> then:
    JFLEX='/usr/share/weird_place/jflex'
You will also need to modify the llvm specific paths:
    LLC='path/to/llc/binary'
    CLANG='path/to/clang/binary'
Once the script knows where to find jflex, llc, and clang,
it should work as expected.</p>

<blockquote>
  <p><em>Note</em> if you have installed the javac and java executables in strange locations,
  you will probably also need to modify <code>JAVAC='javac'</code> and <code>JAVA='java'</code> as well.
  <em>Optional</em> I have included Beaver jar files for convenience, but you 
  may modify BEAVERJAR in run.sh to use a different jar file.</p>
</blockquote>

<h4>Run [Not-So-]Cool Compiler</h4>

<p><code>chmod +x run.sh</code> and run via:
    ./run.sh <filename>
where <code>&lt;filename&gt;</code> is a path to a cool program. For example:
    ./run.sh ../tests/test.cool.cool
The script will compile the parser, followed by the scanner,
followed by the type checker and code generator.
It will then run the driver program, producing <code>main.ll</code>,
and will try to compile <code>main.ll</code> and the <code>driver.c</code> program
to create a machine code executable.
Debugging information is printed to <code>stdout</code>.</p>

<p>Questions or comments? Please email Paul Elliott paul 'd' hs 'd' elliott 'at' gmail 'd' com</p>
