;
;  This file is for implementing the dictionary A.D.T. in Racket
;  This version has the KEY; if you are a student and get it,
;    please let us know!
;
#lang racket
(provide (all-defined-out))

;
; define new-dictionary, dictionary-contains?, etc., here.
;

(define (dictionary-test)
  (display "Begin the dictionary test!\n")
  ;; Choose "Racket->Uncomment" from the DrRacket menu to remove ";" below, if you want to run this test
  (let* ( [d1 (new-dictionary)]
          [d2 (dictionary-add d1 "Pat" 42)]
	  [d3 (dictionary-add d2 "Leslie" 17)]
	  )
    (display (if (or (dictionary? (list "Pat" 42))
    	     	     (not (dictionary? d3))
		     (not (dictionary? d1)))
		 "    Hmmm, some confusion about what is, and is not, a dictionary\n"
		 "dictionary? predicate seems ok\n"))
    (display (if (dictionary-contains? d1 "Pat")
                 "    Wait, how did Pat get here?\n"
                 "d1 looks ok so far\n"))
    (display (if (dictionary-contains? d2 "Pat")
                 "found Pat in d2: "
                 "    Wait, where's Pat?\n"))
    (display (dictionary-ref d2 "Pat"))
    (newline)
    (display (if (dictionary-contains? d3 "Pat")
                 "found Pat in d3: "
                 "    Wait, where's Pat?\n"))
    (display (dictionary-ref d3 "Pat"))
    (newline)
    (display (dictionary-to-code d3)))
    (newline)
    (display "This concludes the dictionary test.\n\n"))



;;;;;;;;;;;;;;;;; KEY IS BELOW ;;;;;;;;;;;;;;;;;;

(define new-dictionary
  (λ ()
    (list 'new-dictionary)))
(define dictionary?
  (λ (what)
     (and (list? what)
          (>= (length what) 1)
	  (or (eq? (first what) 'new-dictionary)
	      (and (eq? (first what) 'dictionary-add)
                   (dictionary? (second what))
                   (string?     (third what)))))))

(define dictionary-empty? (λ (dict) (<= (length dict) 1)))

(define/contract (dictionary-add dict s i)
  (-> dictionary? string? (λ (x) #t) dictionary?) ; allow dictionaries of any type
  (list 'dictionary-add dict s i))

(define dictionary-contains?
  (λ (dict s)
    (if (dictionary-empty? dict)
        #f
        (if (string=? (caddr dict) s)
            (cadddr dict)
            (dictionary-contains? (cadr dict) s)))))
(define dictionary-ref dictionary-contains?)

(define/contract (dictionary-to-code d) ; caution, only works for dictionaries of numbers
  (-> dictionary? string?)
  (if (dictionary-empty? d)
      "(new-dictionary)"
      (string-append "(dictionary-add "
                     (dictionary-to-code (second d))
                     " \"" (third d) "\" "
                     (number->string (fourth d))
                     ")")))
; note that, due to the used of 'dictionary-new' and 'dictionary-add' at the head of those lists,
;            we can also just let racket convert 'd' to text as it would a list

; (dictionary-test)

