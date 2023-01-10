#
# No separate compilation is needed to set up the HERA-C system;
#  these files are just included into HERA programs.
#
# This Makefile just runs some tests.
# The file Make-sample-output.txt should show correct results, you you can run
#     make | diff Make-sample-output.txt -  && make bugtest
# to confirm a proper installation of HERA-C and the associated HERA-libs repo.
# (You should see only the "HERA program is done" confirmation on the error/debug output).
#

test: minitest HERA_main-test altdir_test fardir_test

minitest:
	WARN_HERA_2_3_DIFFS="" HERA_C_DIR=. HERA_C_CPP_DEFS=-DHERA_C_NO_LIB_NEEDED=1 ./HERA-C-Run HERA_nolib-test.hera | tee HERA_nolibneeded-test.out
	WARN_HERA_2_3_DIFFS="" HERA_C_DIR=. ./HERA-C-Run HERA_nolib-test.cc | tee HERA_nolib-test.out
	WARN_HERA_2_3_DIFFS="" HERA_C_DIR=. ./HERA-C-Run HERA_nolib-test.hera | tee HERA_nolib-test-hera.out

HERA_main-test:
	WARN_HERA_2_3_DIFFS="" HERA_C_DIR=. ./HERA-C-Run HERA_main-test.cc | tee HERA_main-test.out
	WARN_HERA_2_3_DIFFS="" HERA_C_DIR=. ./HERA-C-Run HERA_main-test-stacklib.cc | tee HERA_main-test-stacklib.out

bugtest:
	WARN_HERA_2_3_DIFFS="" HERA_C_DIR=. ./HERA-C-Run fixed_bugs.hera | sed -e '/Done with test suite/,$$d' | diff fixed_bugs.txt -

altdir_test:
	mkdir tmp_altdir
	cat  HERA_nolib-test.cc | sed -e 's/nolib-test.hera/nolib-test-moo.hera/' > tmp_altdir/HERA_nolib-test.cc
	(cat HERA_nolib-test.hera ; echo 'SET(R10, 65261)') > tmp_altdir/HERA_nolib-test-moo.hera
	(cd tmp_altdir ; HERA_C_DIR=.. ../HERA-C-Run HERA_nolib-test.cc) | tee HERA_nolib_altdir.out
	rm -r tmp_altdir

fardir_test:
	mkdir tmp_fardir
	cat  HERA_nolib-test.cc | sed -e 's/nolib-test.hera/nolib-test-baa.hera/' > tmp_fardir/HERA_nolib-test.cc
	(cat HERA_nolib-test.hera ; echo 'SET(R10, 61453)') > tmp_fardir/HERA_nolib-test-baa.hera
	HERA_C_DIR=. ./HERA-C-Run tmp_fardir/HERA_nolib-test.cc | tee HERA_nolib_fardir.out
	rm -r tmp_fardir


# test system install
systest:
	WARN_HERA_2_3_DIFFS="" HERA-C-Run HERA_nolib-test.hera | diff HERA_nolib-test-hera.out -
	WARN_HERA_2_3_DIFFS="" HERA-C-Run HERA_main-test.cc | diff HERA_main-test.out -
	WARN_HERA_2_3_DIFFS="" HERA-C-Run HERA_main-test-stacklib.cc | diff HERA_main-test-stacklib.out -
	WARN_HERA_2_3_DIFFS="" HERA-C-Run fixed_bugs.hera | sed -e '/Done with test suite/,$$d' | diff fixed_bugs.txt -
