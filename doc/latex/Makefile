# Makefile for pdflatex documents
#  Author:   Christian Stueble <stueble@acm.org>
#  Version:  $Id: Makefile,v 1.12 2002/06/28 12:47:21 steiner Exp $
#
PRIMTARGET  = tpmmanager

# Path to templates of Sirrix AG. I suggest to set an absolut path using an
# environment variable using 'export SIRRIXTEMPLATES=...'.
ifndef SIRRIXTEMPLATES
        SIRRIXTEMPLATES = /home/anoosheh/projects/templates
endif

# site specific variables
PDFLATEX=pdflatex
BIBTEX=bibtex -min-crossrefs=5
MAKEINDEX=makeindex -s ${SIRRIXTEMPLATES}/latex/styles/ignoreat.ist
MAKEGLOSSARY=makeglos.pl
# we need xfig/transfig >= 3.2.3c to prevent bug with psfrag
RM=rm -f
CP=cp -f
ED=ed
DIFF=diff
ENV=env
TOUCH=touch
DIRSEP=/
PATHSEP=:
ifdef PWD
	# note that we put already here the DIRSEP so we later don't have to add it ..
	TOPDIR=${PWD}${DIRSEP}
else
	# .. and can tolerate a relative path if PWD doesn't exist
	TOPDIR=
endif

## Local configuration file 'make.loc'
##
## Either the Makefile in the package or the optional make.loc file can redefine above
## you might also like define TEXINPUTS there ...
##
## Windows users 
## In particular Windows users probably will have to define 'PATHSEP=\;' and 'VIEWDVI=yap'.
## (for MiKTeX. it also seems that the MiKTeX's 'bibtex' doesn't understand 'BIBINPUTS';
## you might have to edit your tex .ini file and add the directory ....\bibtex to your path ...
## you might have to define TOPDIR
-include make.loc

# You shouldn't need to change anything below this.
# If you did, the change probably needs to be reflected in all such files.


# various standard definitions
#------------------------------
# various directories
SRCDIR      = ${TOPDIR}src
STYLEDIR    = ${TOPDIR}styles
BIBDIR      = ${TOPDIR}bibtex
FONTDIR     = ${TOPDIR}fonts
IMAGEDIR    = ${TOPDIR}images

# fix latex and bibtex invocation to match the paths ...
TEXINPUTS  := .$(PATHSEP)$(SRCDIR)$(PATHSEP)$(STYLEDIR)$(PATHSEP)$(IMAGEDIR)$(PATHSEP)$(BIBDIR)$(PATHSEP)$(PATHSEP)$(TEXINPUTS)$(PATHSEP)$(SIRRIXTEMPLATES)//$(PATHSEP)
MFINPUTS   := $(FONTDIR)$(DIRSEP)$(DIRSEP)$(PATHSEP)$(MFINPUTS)
PDFLATEX   := $(ENV) MFINPUTS=$(MFINPUTS) TEXINPUTS=$(TEXINPUTS) $(PDFLATEX)

GLOSSBIBTEX   :=$(ENV) BSTINPUTS=$(GLOSSBSTINPUTS) BIBINPUTS=$(GLOSSBIBINPUTS) $(BIBTEX)

BIBINPUTS:=$(BIBDIR)$(PATHSEP)$(BIBINPUTS)$(PATHSEP)$(SIRRIXTEMPLATES)/latex/bibtex
BSTINPUTS:=$(BIBDIR)$(PATHSEP)$(BSTINPUTS)
BIBTEX   :=$(ENV) BIBINPUTS=$(BIBINPUTS) BSTINPUTS=$(BSTINPUTS) $(BIBTEX)
# all modified environments
NEWENVDEF = \
	BSTINPUTS=$(GLOSSBSTINPUTS)\
	BIBINPUTS=$(GLOSSBIBINPUTS)$(PATHSEP)$(BIBINPUTS)\
	MFINPUTS=$(MFINPUTS) TEXINPUTS=$(TEXINPUTS) 

# the prime source/target
ifdef PRIMSOURCE
  # PRIMTARGET is in the default case defined but maybe somebody might have a non-standard
  # locations for PRIMSOURCE and has defined PRIMSOURCE instead of PRIMTARGET
  PRIMTARGET= $(notdir $(PRIMSOURCE:%.tex=%))
endif
ifndef PRIMSOURCE
  # somebody might have defined it for non-standard locations of PRIMSOURCE
  PRIMSOURCE  = $(SRCDIR)$(DIRSEP)$(PRIMTARGET).tex
endif

# common sources
STYLESOURCES= $(wildcard $(STYLEDIR)$(DIRSEP)*.sty)
TEXSOURCES  = $(wildcard $(SRCDIR)$(DIRSEP)*.tex)
BIBSOURCES  = $(wildcard $(BIBDIR)$(DIRSEP)*.bib)

# common targets based on primtarget
PDFTARGET      = $(PRIMTARGET).pdf
BIBTARGET      = $(PRIMTARGET).bbl
INDEXTARGET    = $(PRIMTARGET).ind
GLOSSARYTARGET = $(PRIMTARGET).gls
ACRONYMTARGET  = $(PRIMTARGET).acn

# fixpoint temp-file
PRIMFIXPOINT= $(PRIMTARGET).fpt


# the 'exported' target-rules
# -------------------------------------
# Our primary goal is to make the final DVI file
all:: $(PDFTARGET) 

# clean up temp file
clean::
	$(RM) *.aux *.toc *.lof *.lot *.blg *.bbl *.cb *.idx *.ilg *.ind *.ent \
              *.dvi *~ *.log err *.fpt head.tmp body.tmp *.out *.brf *.glo *.gls \
              *.ist *.glg *.acr *.alg *.acn $(PRIMTARGET).pdf

# clean up also "final" generated files
veryclean:: clean
	$(RM) $(PDFTARGET)

# clean up all generatable files, regardless of target
distclean:: veryclean
	$(RM) *.ps *.dvi feymr10.*

# the 'hidden' rules
# -------------------------------------

# BIBTEX specific targets
# biboutput depends on the bib-sources as well as the tex-files
# as this target is after the main dvi target latex should have been run already once so
# aux file should exist (unfortunatly we cannot make a target on .aux or otherwise we will
# never consider everything done
# Ideally bibtex should only be run if texsources changed bib-references. But this seems
# rather involved to figure out (e.g. diff of aux-files before/after and grep for
# 'citation|bibcite|bibdata|bibstyle' ...)
$(BIBTARGET):: $(BIBSOURCES) $(TEXSOURCES)
	$(BIBTEX) $(PRIMTARGET) || ( $(RM) $@; $(RM) $(PRIMFIXPOINT); exit -1)
	$(RM) $(PRIMFIXPOINT) 	# we have to start 'fixpoint calculation' from scratch

# if you just would like to run bibtex once (e.g for debugging)
Bibtex:
	$(BIBTEX) $(PRIMTARGET) 


# INDEX specific target
# index depends only texsource (again the usual thing about not being able to target *.idx)
# Ideally makeindex should only be run only if texsources changed index related stuff.
# But this seems rather involved to figure out (e.g. diff of $(PRIMTARGET).idx file)
Makeindex:
	# fix some index problems
	# ed/vi replace expressions are
	# - '1,$s/\(see{.*\)!/\1, /g'           # newabbrev (see thesis-common.sty for more information)
	# - '1,$s/\([^"]\)\("0[0-9]*\)/\1"\2/g' # Math accent signs
	# - '1,$s/\([^"]\)\(!}\)\)/\1"\2/g'     # some !'s
	-echo -e '1,$$s/\(see{.*\)!/\\1, /g\n1,$$s/\([^"]\)\("0[0-9]*\)/\\1"\\2/g\n1,$$s/\([^"]\)\(!}\)/\\1"\\2/g\nw\nq' | $(ED) $(PRIMTARGET).idx
	$(MAKEINDEX) $(PRIMTARGET)

$(INDEXTARGET):: $(TEXSOURCES) 
	# fix some index problems (see above)
	-echo -e '1,$$s/\(see{.*\)!/\\1, /g\n1,$$s/\([^"]\)\("0[0-9]*\)/\\1"\\2/g\n1,$$s/\([^"]\)\(!}\)/\\1"\\2/g\nw\nq' | $(ED) $(PRIMTARGET).idx
	$(MAKEINDEX) $(PRIMTARGET)
	$(RM) $(PRIMFIXPOINT) 	# we have to start 'fixpoint calculation' from scratch

$(GLOSSARYTARGET):: $(TEXSOURCES)
	$(MAKEGLOSSARY) $(PRIMTARGET)

$(ACRONYMTARGET):: $(TEXSOURCES)
	$(MAKEGLOSSARY) $(PRIMTARGET)


# pdflatex version
# make in sure that we always run pfdlatex (for fixpoint!)
# primarily .pdf file depends on the main tex source files plus all other tex-files we find
$(PDFTARGET):: $(PRIMSOURCE) $(TEXSOURCES) $(STYLESOURCES) 
	$(PDFLATEX) $(PRIMSOURCE) || ($(RM) $@; exit -1)

# additionaly it depends on bibliography, glossary and index. keep this target separate so
# the first latex is run before this target is resolved 
$(PDFTARGET):: $(BIBTARGET) $(INDEXTARGET)

$(PDFTARGET):: $(GLOSSARYTARGET)

$(PDFTARGET):: $(ACRONYMTARGET)

# and we also depend on the fixpoint
# '$(PDFLATEXTARGET):: $(PRIMFIXPOINT)' does not work as we have to make sure that
# the fixpoint is calculated with pdflatex and not latex!
$(PDFTARGET)::
	$(MAKE) LATEX="$(PDFLATEX)" $(PRIMFIXPOINT)

# if you just would like to run pdflatex once (e.g for debugging)
Pdflatex:
	$(PDFLATEX) $(PRIMSOURCE)


# FIXPOINT targets
# after we have run bibtex we have to run latex again. as one latex run might not be
# enough to correctly resolve all references we run latex after bibtex until .aux file
# stays constant.
# This is not optimal as we might run latex one time too often but at least all references should
# be resolved afterwards (at least as much as possible)
$(PRIMFIXPOINT): 
	# Save old aux-file state (Note that we cannot only do it for the PRIMTARGET
	# as there might be \includes with separate aux-files!)
	# Unfortunatly the make-functions are evaluated only the first time when
	# potentially the aux-files don't exist. So make sure that at least
	# $(PRIMTARGET).aux (which has to exist!) is there. Almost definitly
	# the diff will fail and in the following recursion all aux-files will
	# then be found ...
	for file in $(PRIMTARGET) $(patsubst %.aux,%,$(wildcard *.aux)); do \
	   $(CP) $$file.aux $$file.fpt; \
	done
	$(LATEX) $(PRIMSOURCE) || ($(RM) $@; exit -1)	# run latex
        # if aux-files changed we have to recurse
	for file in $(PRIMTARGET) $(patsubst %.aux,%,$(wildcard *.aux)); do \
	    $(DIFF) -q $$file.aux $$file.fpt || \
	    ($(RM) $(PRIMFIXPOINT); $(MAKE) $(PRIMFIXPOINT)); \
	done

# create empty aux-files. usefull to be able to run with \includeonly
# selected parts.
aux: $(notdir $(TEXSOURCES:%.tex=%.aux))

%.aux:
	$(TOUCH) $@

