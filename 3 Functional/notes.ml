(* Assertions make sure something is true or raise an error *)
let e = true;;
assert e;;

(* Operations *)
2 + 3;;
2.2 +. 3.3;;
7 = 4;;

(* If Expressions *)
4 + (if 'a' = 'b' then 1 else 2);;

if 'a' = 'b' then print_endline "yep cock";;
else if a = 'c' then print_endline "no cock";;
else print_endline "maybe cock";;

(* Has Type *)
let x = 42;;
val x : int
