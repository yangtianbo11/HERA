#lang racket

(require "test-tree.rkt")

(if (tree-test)
	(display "tree test looks good\n")
	(display "Rats! Something wrong somewhere in tree test\n"))
