# generated 2005/12/14 20:48:47 SGT by fabrice@amra.dyndns.org.(none)
# using glademm V2.6.0

SUBDIRS = po Utils Tokenize SQL Collect @SOAP_SUBDIRS@ Search \
	Index Monitor UI/RenderHTML UI/GTK2/src

EXTRA_DIST = ChangeLog NEWS README TODO mkinstalldirs pinot.desktop \
	pinot.spec textcat_conf.txt Search/Plugins/*src Search/Plugins/*.xml \
	Collect/pinot-collect.1 Search/pinot-search.1 UI/GTK2/src/pinot.1 \
	UI/GTK2/index.html UI/GTK2/xapian-powered.png UI/GTK2/pinot.png \
	UI/GTK2/metase-gtk2.glade UI/GTK2/metase-gtk2.gladep

man_MANS = Collect/pinot-collect.1 Search/pinot-search.1 UI/GTK2/src/pinot.1

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(datadir)/pinot
	$(INSTALL_DATA) UI/GTK2/index.html $(DESTDIR)$(datadir)/pinot/index.html
	$(INSTALL_DATA) UI/GTK2/xapian-powered.png $(DESTDIR)$(datadir)/pinot/xapian-powered.png
	$(INSTALL_DATA) UI/GTK2/metase-gtk2.glade $(DESTDIR)$(datadir)/pinot/metase-gtk2.glade
	$(INSTALL_DATA) UI/GTK2/metase-gtk2.gladep $(DESTDIR)$(datadir)/pinot/metase-gtk2.gladep
	$(INSTALL_DATA) textcat_conf.txt $(DESTDIR)$(datadir)/pinot/textcat_conf.txt
	$(mkinstalldirs) $(DESTDIR)$(datadir)/pinot/engines
	$(INSTALL_DATA) Search/Plugins/*.src $(DESTDIR)$(datadir)/pinot/engines/
	@mv $(DESTDIR)$(datadir)/pinot/engines/AmazonAPI.src $(DESTDIR)$(datadir)/pinot/
	$(INSTALL_DATA) Search/Plugins/*.xml $(DESTDIR)$(datadir)/pinot/engines/
	$(mkinstalldirs) $(DESTDIR)$(datadir)/pinot/tokenizers
	@rm -f $(DESTDIR)$(libdir)/lib*tokenizer.*
	$(INSTALL_DATA) Tokenize/.libs/*.so $(DESTDIR)$(datadir)/pinot/tokenizers/
	$(mkinstalldirs) $(DESTDIR)$(datadir)/icons/hicolor/48x48/apps/
	$(INSTALL_DATA) UI/GTK2/pinot.png $(DESTDIR)$(datadir)/icons/hicolor/48x48/apps/pinot.png
	$(mkinstalldirs) $(DESTDIR)$(datadir)/applications
	@desktop-file-install --vendor Amra --dir $(DESTDIR)$(datadir)/applications pinot.desktop

ACLOCAL_AMFLAGS = -I m4

