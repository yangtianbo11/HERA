;
;  This file is for implementing the calculator in Racket
;
#lang racket
(provide run-calculator)
(require "dictionary.rkt")



; first, a convenient calculator-trace function that prints output but can
;  all be turned off with this calculator-trace-on variable

(define calculator-trace-on #f) ; set to #f when not watching it tell you what its doing

(define (calculator-trace string)
  (if calculator-trace-on
      (begin (display string) #t)
      #t))  ; don't actually care about the returned value, but Racket wants an 'else'


(define (run-calculator stop-on-error)
  (printf "Welcome to the awesome CS245 desk calculator (Racket edition),\n")
  (printf "An amazing app by N. Swerki.\n")
  ; (printf (" (with additional help from ******* and ******* and ******)\n")
  (printf "\n Enter input for the calculator, with each entry on its own line, and 'bye' when you're done.\n")
  (repl (new-dictionary) (new-dictionary) stop-on-error))


(define ops
  (dictionary-add
   (dictionary-add
    (dictionary-add
     (dictionary-add
      (dictionary-add (new-dictionary) "+" +)
      "-" -)
     "*" *)
    "<=" (λ (l r) (if (<= l r) 1 0)))
   "==" (λ (l r) (if (= l r) 1 0))))

(define (value-of c v name)
  (if (string->number name)
      (string->number name)
      (if (dictionary-contains? c name)
          (dictionary-ref c name)
          (if (dictionary-contains? v name)
              (dictionary-ref v name)
              (begin
                (printf " >>> calculator error, unrecognized variable name  '~s', treated as 0.\n" name)
                0)))))

; Here we implement a Read-Eval-Print-Loop using tail-recursion in Racket.
; 
; This function will repeatedly read and run input lines, via a tail-call to itself. So...
;  when we see "bye", we return #t (true)
;  when we see something valid, we respond and call (repl) to keep going
;  when we something invaled but want to keep going, we print an error message and call (repl)
;  when we see something so unworkable that we want to stop, e.g. an unimplemented case, we return #f
;
; Notes and challenges:
;  * If you want the responses to _change_ as the user does things such as "const x 5 ;",
;    then different calls will have to respond differently. So, "repl" will need a parameter...
;
;  * As in the C++ version, if there are 5 operations and 4 possible mixtures of constants/variables,
;    there may be an obvious way to write code that repeats essentially similar elements 20 times over.
;    Can you get that down to four or five repetitions? Two? One?
; 
(define (repl consts vars stop-on-error)
  ; Do something here to read the next line, split up each command,
  (display " C> ")  ; provide a Calculator prompt, a bit different from Racket's
  ; Note: we simply include this to demonstrate how to read a
  ; line. This implementation is incomplete, and will not function
  ; properly (updating the stack) until you implement it correctly.
  (let* ( [input (read-line)]
          [tokens (if (string? input) (string-split input) #f)] )

    (calculator-trace (format "got tokens ~a\n" tokens))
    
    ; Note that Racket's "cond" is like if/elif/elif/else in Python,
    ;       see https://docs.racket-lang.org/reference/if.html
    (cond
      [(or          ; this is the "test" of the if/elif/...  did we get no tokens, or "bye"?
	(not tokens)
	(and (>= (length tokens) 1) (string=? (first tokens) "bye")))
	
       (begin       ; this is what gets done if the test passes ... here, we return #t for successful finish
         (printf "OK, bye\n")
         #t)]


      ; here are the next test/result pair in our if/elif/... sequence
      [(and (= (length tokens) 4) (dictionary-contains? ops (second tokens)) (string=? (fourth tokens) ";"))  ; 4 parts ending with a ";"
       (begin
         (printf "~a\n"  ((dictionary-ref ops (second tokens))
                          (value-of consts vars (first tokens))
                          (value-of consts vars (third tokens))))
         (repl consts vars stop-on-error))  ;; *** Here's where we "go around the loop" again
       ]

      [(and (= (length tokens) 4)
            (or (string=? (first tokens) "var") (string=? (first tokens) "const"))
            (string=? (fourth tokens) ";"))
       (let ([new-name (second tokens)]
             [init     (value-of consts vars (third tokens))])
         (if (or (string->number new-name)
                 (dictionary-contains? consts new-name)
                 (dictionary-contains? vars   new-name))
             (begin
               (printf "Sorry, illegal definition of name: ~a." new-name)
               (if stop-on-error #f (repl consts vars stop-on-error)))
             (if (string=? (first tokens) "var")
                 (repl consts
                       (dictionary-add vars   new-name init)
                       stop-on-error)
                 (repl (dictionary-add consts new-name init)
                       vars
                       stop-on-error))))    ]

      [(and (= (length tokens) 4)
            (string=? (first tokens) ":=")
            (string=? (fourth tokens) ";"))
       (let ([old-name (second tokens)]
             [new-val  (value-of consts vars (third tokens))])
         (if (dictionary-contains? vars old-name)
             (repl consts
                   (dictionary-add  vars   old-name new-val)
                   stop-on-error)
             (begin
               (printf "sorry, didn't find a variable ~a to change.\n" old-name)
               (if stop-on-error #f (repl consts vars stop-on-error))))) ]

      [(and (>= (length tokens) 1) (string=? (first tokens) "?"))
       (begin
         ;(dict-print-unique-nicely vars)
         ;(dict-print-unique-nicely consts)
         (display "Sorry, too tired to build another printing function, just printing these in Racket for debugging\n")
         (display (dictionary-to-code vars))
         (display (dictionary-to-code consts))
         (repl consts vars stop-on-error))]

      [else ; huh, I wonder what we got?
       (begin
         (printf "didn't get \"bye\" or 3 tokens and then \";\", ignoring this input: \"~a\"\n" input)
         (if stop-on-error
             (begin (display "giving up, bye") #f)
             (repl consts vars stop-on-error)))]
      )))

; If you want to start the repl, call this:
; (run-calculator #t)
