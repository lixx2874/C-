open Lwt
open Unix
(*Need Unix module for printing out the time*)

module Server = struct
    (* a list associating user nicknames to the output channels that write to their connections *)
    (* Once we fix the other functions this should change to []*)
    let sessions = ref []	     
    exception Quit
		
    (* replace Lwt.return below with code that uses Lwt_list.iter_p to 
       print sender: msg on each output channel (excluding the sender's)*)
    
    let rec broadcast sender msg =
    let time = Unix.time () in
    let {Unix.tm_sec=seconds; tm_min=minutes; tm_hour=hours;
    tm_mday=day_of_month; tm_mon=month; tm_year=year;
    tm_wday=wday; tm_yday=yday; tm_isdst=isdst} =
    Unix.localtime time
    in
	Lwt_list.iter_p (fun (nn,outp) -> if sender != nn then Lwt_io.fprint outp (" <" ^ (string_of_int hours) ^ ":" ^ (string_of_int minutes) ^ ":" ^ (string_of_int seconds) ^ "> " ^ sender ^ ":" ^ msg ^ "\n") else Lwt.return ()) (List.remove_assoc sender !sessions)  

    (* remove a session from the list of sessions: important so we don't try to 
       write to a closed connection *)
    let remove_session nn = 
      sessions := List.remove_assoc nn !sessions;
      broadcast nn "<left chat>" >>= fun () ->
      Lwt.return ()

    (* Modify to handle the "Quit" case separately, closing the channels before removing the session *)
    let handle_error e nn inp outp = match e with
	| Quit -> Lwt_io.close inp >>= fun () -> Lwt_io.close outp >>= fun () -> remove_session nn >>= fun () -> Lwt.return() 
	| _-> failwith "Eh...Something wrong with the server."

    (* modify sessions to remove (nn,outp) association, add (new_nn,outp) association.
       also notify other participants of new nickname *)
    let change_nn nn outp new_nn = 
	broadcast !nn ("<changed nick to "^ new_nn ^">")>>= 
	fun () -> sessions := List.remove_assoc !nn !sessions;
	nn:=new_nn;
	sessions := (!nn,outp)::!sessions;	
	Lwt.return ()

    let chat_handler (inp,outp) =
      let nick = ref "" in
      let side = ref "" in
      (* replace () below with code to 
         + obtain initial nick(name), 
	*)
	let _ = Lwt_io.fprint outp "Welcome to chatting room! Enter initial nickname:" >>= fun () -> nick :=Lwt_main.run(Lwt_io.read_line inp); 
	 Lwt_io.fprint outp "Here is the battle field where war starts! Which side are u in?! Alliance or Horde?"; side := Lwt_main.run(Lwt_io.read_line inp);
	nick := !nick ^ " < " ^ !side ^ " > "; 
	(*+ add (nick,outp) to !sessions, *)
	sessions := (!nick,outp)::!sessions;
	(*and  + announce join to other users *) 
	broadcast !nick "<joined>" in
      (* modify handle_input below to detect /q, /n, and /l commands *)
      let handle_input l = if Str.string_before l 2="/q" then Lwt.fail Quit
	else if Str.string_before l 2="/n" then change_nn nick outp ((Str.string_after l 2)^ " < " ^ !side ^ " > ") 
	else if Str.string_before l 2="/l" then Lwt_list.iter_s (fun (inpp,outpp) -> Lwt_io.write_line outp inpp) !sessions
	
	else broadcast !nick l in
      let rec main_loop () =
	Lwt_io.read_line inp >>= handle_input >>= main_loop in
      Lwt.async (fun () -> Lwt.catch main_loop (fun e -> handle_error e !nick inp outp))
  end

let port = if Array.length Sys.argv > 1 then int_of_string (Sys.argv.(1)) else 16384		  
let s = Lwt_io.establish_server (Unix.ADDR_INET(Unix.inet_addr_any, port)) Server.chat_handler	    
let _ = Lwt_main.run (fst (Lwt.wait ()))
let _ = Lwt_io.shutdown_server s (* never executes; you might want to use it in utop, though *)
