(* Bloom Filter implementation.  This file will not compile as is. *)
module type memset = sig
    type elt (* type of values stored in the set *)
    type t (* abstract type used to represent a set *)
    val mem : elt -> t -> bool 
    val empty : t
    val is_empty : t -> bool
    val add : elt -> t -> t
    val from_list : elt list -> t
    val union : t -> t -> t
    val inter : t -> t -> t
  end

(* Define the hashparam signature here *)	
module type hashparam = sig
	type t
	val hashes: t -> int list
	end

(* Define SparseSet module here, using the Set.Make functor *)			  
module SparseSet =struct
	include Set.Make 
	(struct type t = int
       let compare = Pervasives.compare
  end)
	let from_list lst = List.fold_left (fun result x -> add x result) empty lst
	end 
(* Fill in the implementation of the memset signature here.  You'll need to expose the elt type *)		       
module BitSet:memset with type elt = int = struct
    (* Some helper functions... bitwise &, bitwise | of two char values: *)
    type elt = int
    type t = string
    let (&*) c1 c2 = String.make 1 (Char.chr ((Char.code c1) land (Char.code c2)))
    let (|*) c1 c2 = String.make 1 (Char.chr ((Char.code c1) lor (Char.code c2)))	  
    (* bitwise or of two strings: *)
    let rec (|@) s1 s2 = match (s1,s2) with
      | ("",s) | (s, "") -> s
      | _ -> (s1.[0] |* s2.[0]) ^ ((Str.string_after s1 1) |@ (Str.string_after s2 1))
    let rec (&@) s1 s2 = match (s1,s2) with
      | ("",s) | (s, "") -> ""
      | _ -> (s1.[0] &* s2.[0]) ^ ((Str.string_after s1 1) &@ (Str.string_after s2 1))
    (* single-character string with bit i set: *)
    let strbit i = String.make 1 (Char.chr (1 lsl (i land 7)))
    let rec make_str_t num str = if (num < 8) then str ^ (strbit num) else make_str_t (num-8) ("\000"^str) 
    let empty = ""
    let is_empty str = (str=String.make (String.length str) '\000')
    let mem x str = ((String.length str) > x/8 ) && ((Char.code str.[x/8]) land (1 lsl (x land 7)) <>0 )
    let add x str = str |@ (make_str_t x "")
    let union str1 str2 = (|@) str1 str2
    let inter str1 str2 = (&@) str1 str2
    let from_list lst = List.fold_left (fun result x -> add x result ) empty lst 
  end

(* Fill in the implementation of a BloomFilter, matching the memset signature, here. *)
(* You will need to add some sharing constraints to the signature below. *)
module BloomFilter(S : memset with type elt = int)(H : hashparam) : (memset with type elt = H.t) = struct
    type elt = H.t
    type t =  S.t
    let empty = S.empty
    let is_empty lst = S.is_empty lst
    let mem x lst = List.fold_left (fun result x -> if S.mem x lst then result&&true else false) true (H.hashes x)
    let add x lst = List.fold_left (fun result x-> S.add x result) lst (H.hashes x)
    let union lst1 lst2 = S.union lst1 lst2
    let inter lst1 lst2 = S.inter lst1 lst2
    let from_list lst = List.fold_left (fun result x -> add x result) S.empty lst
  end

(* TA COMMENT(dacos014): 

   * 2.0 / 2.0 pts for correct definition. This means using memset (0.5), hashparam (0.5)
     and setting elt type to that of hashparam's t (0.5). Additional 0.5 for overall correct definition.
   * 0.5 / 0.5 pts for correct t and elt types.
   * 0.5 / 0.5 pts for correct union declaration.
   * 0.5 / 0.5 pts for correct inter declaration.
   * 1.0 / 1.0 pts for correct mem declaration.
   * 0.5 / 0.5 pts for using memset's empty as empty.
   * 0.5 / 0.5 pts for using memset's is_empty as is_empty.
   * 2.0 / 2.0 pts for correct add function. This means using memset's add (0.5), hashparam's hashes (0.5),
     as well as list.fold_left (0.5). Additional 0.5 for overall correct function.
   * 2.5 / 2.5 pts for correct from_list declaration. **Maximum 1.5 pts for attempt.**
   
*)

(* A hashparam module for strings... *)
module StringHash = struct
    type t = string (* I hash values of type string *)
    let hlen = 15
    let mask = (1 lsl hlen) - 1
    let hashes s =
      let rec hlist n h = if n = 0 then [] else (h land mask)::(hlist (n-1) (h lsr hlen)) in
      hlist 4 (Hashtbl.hash s) 
  end

(* Add the IntHash module here *)
module IntHash:hashparam with type t = int = struct
    type t = int
    let hashes n = let h1 a= ((795*a + 962) mod 1031) in 
		let h2 b= ((386*b + 517) mod 1031) in
		let h3 c=  ((937*c + 693) mod 1031) in
		[(h1 n); (h2 n); (h3 n)]
  end 

(* TA Comment(moham775) Int Hash: 5/5 *)

(* TA Comment(meye2058) BitSet Module Feedback: 10/10 *)
