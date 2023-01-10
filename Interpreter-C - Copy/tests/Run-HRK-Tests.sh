SEDFILTER='s/_/_/'
POST_TEST=""

if test "$1" = "Racket"
then
    CALCEXE=~/cs245/Interpreter-Racket/main.rkt
    RUNCALC="racket ${CALCEXE}"
    echo "# Running tests with racket project"
else
    CALCEXE=~/cs245/Interpreter-C++/Debug/Interpreter-C++
    RUNCALC="${CALCEXE}"
    if test -x ${CALCEXE}
    then
       echo "# Running tests with C++ project"
    else
	echo "Can't run $CALCEXE ... if using C++, did you do a \"Clean Project\" and \"Build Project\" without errors?"
	exit 111
    fi
fi

ulimit 8

for test in /home/courses/examples/cs245/Racket-tests/*.hrk
do
    echo " "
    echo " ====== testing $CALCEXE on example $test ====== "
    if test -r `echo $test | sed -e 's/\.hrk$/.out/'`
    then
	echo "        Expect the output:"
	cat `echo $test | sed -e 's/\.hrk$/.out/'` | sed -e 's/^/        /'
	echo "        ======"
    fi
    ( cat $test ; echo ${POST_TEST} ) | $RUNCALC | sed -e "${SEDFILTER}"
done

