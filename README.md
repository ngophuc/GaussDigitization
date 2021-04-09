# GaussDigitization

To install the program see <a href="https://github.com/ngophuc/GaussDigitization/blob/master/INSTALL.txt">INSTALL.txt</a> file

# Program execution

<code>./GaussDigitization input_file [3D]</code>

- 2D : The programme takes as input a text file containing the sequence of points (some examples are given in <a href="https://github.com/ngophuc/GaussDigitization/tree/main/Samples">Samples</a> file).

- 3D : The programme takes as input an off file containing a mesh(some examples are given in <a href="https://github.com/ngophuc/GaussDigitization/tree/main/Samples">Samples</a> file).

The programme outputs a text file, named **output.txt**, containing the ditized set of points. 

If you activate DGtal, then it outputs :
- 2D : an image in svg format, named **output.svg** (some examples are given in <a href="https://github.com/ngophuc/GaussDigitization/tree/main/Results">Results</a> file).
- 3D : visualisation with voxels (some examples are given in <a href="https://github.com/ngophuc/GaussDigitization/tree/main/Results">Results</a> file).

# Examples

- Example of exection in 2D :

<code>./GaussDigitization ../Samples/circle10.2d</code>

<code>./GaussDigitization ../Samples/ellipse.2d</code>

<code>./GaussDigitization ../Samples/flower.2d</code>

<code>./GaussDigitization ../Samples/hand.2d</code>

<code>./GaussDigitization ../Samples/leaf.2d</code>

- Example of exection in 3D :

<code>./GaussDigitization ../Samples/camel.2d</code>

<code>./GaussDigitization ../Samples/bunny.2d</code>

<code>./GaussDigitization ../Samples/blobby.2d</code>
