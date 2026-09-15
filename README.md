# c-bmp-viewer
Very simple (and bad) BMP viewer (specifically monochrome/1 bit)

I only have a few days of experience in C, and I thought I could try my hand at making an image viewer. PPM seemed too simple, and something like JPG, PNG, and even BMP with compression was a bit too hard for me (for now...), so I've opted for BMP with no compression and monochrome only that can be viewed in the terminal (no graphics because I don't know how to do that yet).

Pretty sure some of it is still wrong, such as the height is stored bottom up, left to right, so that would explain why my image is upside down. (and more)

Overall it was quite difficult (mostly because I didn't have a clue on what to do aside from reading the headers at first), however it was fun. I have learned a lot, and maybe in the future once I get better at C (and become more knowledgeable on image formats), I will redo this but with compression, multiple bit depths supported, etc.

Note the ``colourTable`` is kind of redundant in my code since I display everything in the terminal, but I retrieved it anyway to learn.