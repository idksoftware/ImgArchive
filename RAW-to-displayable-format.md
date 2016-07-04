###RAW to displayable format
Camera RAW format as produced by most cameras cannot be directly displayed by most file and web browsers. This means the image contents is hidden, and in most cases just a empty icon is displayed. One way to mitigate this problem would be to create a displayable image from the RAW image, in much the same way that cameras can create a RAW plus JPG pair. SIA cannot create this displayable image itself, however third party tools such as Imagemagic can. For SIA to use such tools SIA provides hooks. These hooks call hook scripts that call the correct third party tool that will create the displayable image.

   




