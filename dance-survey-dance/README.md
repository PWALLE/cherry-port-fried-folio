<h1>README</h1>

<p>@author: Paul Elliott <br>
@date: 11/8/14</p>

<h3>About</h3>

<p>This directory contains source from an independent contract project that I did
for a friend, Tina. Tina is a graduate student in cultural arts. She has many
talents, but math and statistics is not one of those talents. For her gradaute
thesis, she conducted a neat survey at two major museums. But, when she got
the results back--nearly <em>400</em> pages of surveys--her head got a little fuzzy.</p>

<p>So, I told her I would build a program that allowed her to brainlessly enter
the data from her surveys and store it for later processing. I built the
first <code>Main.py</code> and support files for this data entry job--keeping in mind
all the various pitfalls a computer-illiterate, overworked and very tired
student might encounter. With this program, Tina cranked out the data entry
in less than <em>3 hours</em>. She even said she enjoyed it! (I think she was lying).</p>

<p>Now with data in hand (or, well, computer in hand and data within), Tina
wanted to run some simple calculations. The second driver, <code>Data.py</code>, was
born. Tina finished her thesis and was happy ever after. I got some free coffee.</p>

<p><strong>This code is not designed to be a widely deployed tool--it was created
in the interest of helping one graduate student, exhibiting some minimal
homage to the Pythonista convention wizards..and get some free coffee.</strong>
~PWALLE</p>

<h3>Notes</h3>

<p>There are two main programs here:</p>

<ol>
<li><code>Main.py</code> for data entry</li>
<li><code>Data.py</code> for statistic calculations</li>
</ol>

<p>The first is largely a command line data entry tool. Before running,
look at <code>Constants.py</code> to modify some env variables, like the data directory
(though my program will create one if you don't specify) and the survey
characteristics (question types and numbers).</p>

<p>To run:</p>

<blockquote>
  <p><code>python Main.py</code></p>
</blockquote>

<p>This starts the data entry loop, which is operated via keyboard commands.
I anticipated many mistyped answers, so 'b' functions as a 'back' button
to return the user to the previous question.</p>

<p>The second driver runs merely on its own..make config changes within the block:</p>

<blockquote>
  <p><code>if __name__=="__main__":</code></p>
</blockquote>
