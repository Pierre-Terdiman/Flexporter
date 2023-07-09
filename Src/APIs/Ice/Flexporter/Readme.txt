The Flexporter API itself doesn't depend on a particular DCC tool.
So it should be possible to port Flexporter to Maya/Carrara quite
easily.

This is the internal API, not exposed to users. This is used by
the DCC plug-ins (e.g. a MAX exporter), to implement the actual
exporters.

This is not the same as the Flexporter SDK, used by users to do
plug-ins for the DCC tool's plug-ins...

In short:

- use the Flexporter API to create exporters for MAX, Maya,
  Carrara, whatever

- use the Flexporter SDK to create format plug-ins for Flexporter.
  Those ones then automatically with all flavours of Flexporter,
  i.e. with all DCC tools that support Flex.

So, in theory, a user can create his own format and automatically
have it understood by MAX, Maya, etc.

In practice, it has only been done with MAX and Carrara for ZCB
files.