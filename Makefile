# ********* slapwm Makefile **********

PREFIX         = /usr/local/slap
PREFIX_BIN     = $(PREFIX)/bin
PREFIX_SHARE   = $(PREFIX)/share
PREFIX_ETC     = $(PREFIX)/etc/slap
PREFIX_THEMES  = $(PREFIX_SHARE)/themes/slap


CC                 = g++
IMLIB2_CONFIG      = imlib2-config
FREETYPE2_CONFIG   = freetype-config
XML2_CONFIG        = xml2-config
CP 		   = cp -vf
INSTALL		   = install


# change this if you don't have pentium4
DEBUG_CFLAGS   = -g -Wall
SMALL_CFLAGS   = -fno-rtti -fno-exceptions -Wall
CFLAGS         = $(DEBUG_CFLAGS) \
		-DSLAP_HOME=\"$(PREFIX)\" \
		-D_XML_KEY_SIZE=255


XROOT    = $(DESTDIR)/usr/X11R6
INCLUDES = -I$(XROOT)/include `$(IMLIB2_CONFIG) --cflags` `$(FREETYPE2_CONFIG) --cflags` `$(XML2_CONFIG) --cflags` 
LDPATH   = -L$(XROOT)/lib 
LIBS     = -lX11 -lXft -lXext `$(IMLIB2_CONFIG) --libs` `$(FREETYPE2_CONFIG) --libs` `$(XML2_CONFIG) --libs`  
OBJS     = linkedlist.o events.o canvas_item.o canvas_button_x11.o canvas_image.o  canvas_button_image.o canvas_rect.o canvas.o main.o
RM	  = rm -f


all: canvastest

canvastest: $(OBJS)
	$(CC) $(OBJS) $(LDPATH) $(LIBS) -o $@

$(OBJS): %.o: %.cc $(HEADERS)
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

clean: Makefile
	$(RM) $(OBJS) *~ canvastest


install_themes:

install_main:
	$(INSTALL) -m 0755 -d $(PREFIX_BIN)
	$(INSTALL) -m 0755 -d $(PREFIX_ETC)
	$(INSTALL) -m 0755 canvastest $(PREFIX_BIN)

install: all install_main 

