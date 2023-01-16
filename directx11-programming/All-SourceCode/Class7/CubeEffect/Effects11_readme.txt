ReadMe for Effects11_nn.lib:
============================

DirectX 11 does not provide the Effects implementation along with the other libraries.
It is provided in the DirectX SDK under "Samples\C++\Effect11\" as a static library.
We must compile it and link to it. We have compiled the "Release configurations". They
are provided here as "Effects11_32.lib" and "Effects11_64.lib".

The CubeEffect sample links to "Effects11_32.lib". It also uses the Include Directory
"$(DXSDK_DIR)Samples\C++\Effects11\Inc".
