open Bloom

module BloomTest (S:memset) = struct 
    let test lst1 lst2 s = 
	let start1 = Sys.time () in
	let set = S.from_list lst1 in 
	let finish1 = Sys.time () in 
	let time1 = (finish1 -. start1) in 
	let start2 = Sys.time () in
	let false_positives = List.fold_left (fun acc e -> if S.mem e set then acc+1 else acc) 0 lst2 in
	let finish2 = Sys.time () in
	let time2 = (finish2 -. start2) in
	Printf.printf "%sbuild time = %fs test time =    %fs false positives = %d\n" s time1 time2 false_positives
end 

module BloomSparseInt = BloomFilter (SparseSet) (IntHash)
module BloomBitInt = BloomFilter (BitSet) (IntHash)

let insert_list = 
	let rec help lst num = 
		if num > 0 then (Random.int ((1 lsl 30)-1))::(help lst (num-1)) else [] in help [] 200


let test_list =
	let rec help lst num = match num with
		| 0 -> lst
		| _ -> help (Random.int ((1 lsl 30)-1)::lst) (num-1)
  in help [] 1000000


module SparseIntTest = BloomTest (BloomSparseInt)
let sparseinttest = SparseIntTest.test insert_list test_list "Sparse Int       :"

module BitIntTest = BloomTest (BloomBitInt)
let bitinttest = BitIntTest.test insert_list test_list "Bit Int         :"

let read_input_lines filename = 
    let in_file = open_in filename in
    let rec loop acc =
        let next_line = try Some (input_line in_file) with End_of_file -> None in
        match next_line with
            | (Some l) -> loop (l::acc)
            | None -> acc
        in
    let lines = try List.rev (loop []) with _ -> [] in
    let () = close_in in_file in
    lines

let insert_list = read_input_lines "top-2k.txt"
let test_list = read_input_lines "top-1m.txt"

module BloomSparseString = BloomFilter (SparseSet) (StringHash)
module BloomBitString = BloomFilter (BitSet) (StringHash)


module SparseStringTest = BloomTest (BloomSparseString)
let sparsestringtest = SparseStringTest.test insert_list test_list "Sparse String     :"

module BitStringTest = BloomTest (BloomBitString)
let bitstringtest = BitStringTest.test insert_list test_list "Bit String       :"


(*TA Comment halto004:
  -Correct tests present: 5/5
  -Using correct modules: 4/4

*)
(* TA COMMENT (leid0065): 6/6 *)
