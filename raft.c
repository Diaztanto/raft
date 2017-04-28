typedef struct {
	int type;
	int value;
} message;

void raft_activity() {
	while (1) {
		if follower() {
			wait_until_receive_or_timeout();
			if recieve() {
				if (recieved->type = APPEND_ENTRIES) {
					leader = recieved->sender;
				} else if (recieved->type = LOG_ORDER) {
					add_log(recieved);
				} else if (recieved->type = LOG_CONFIRMATION) {
					value = recieved->value;
					commit_log();
				} else if (recieved->type = VOTE_REQUEST) {
					if (leader != null) {
						leader = null;
						send_vote(recieved->sender);
					}
				} else {
					throw_exception();
				}
				reset_timeout();
			} else if timeout() {
				declare_self_candidate();
				vote = 1;
				send_vote_request();
			} else {
				throw_exception();
			}

		} else if candidate() {
			wait_until_receive_or_timeout();
			if recieve() {
				if (recieved->type = VOTE) {
					vote++;
					if (vote > 0.5*nodeCount) {
						declare_self_leader();
					}
				} else if (recieved->type = VOTE_REQUEST) {
					//Do nothing
				} else if (recieved->type = APPEND_ENTRIES) {
					declare_self_follower();
					leader = recieved->sender;
				} else {
					throw_exception();
				}
			} else if timeout() {
				vote = 1;
				send_vote_request();
			} else {
				throw_exception();
			}
		} else if leader() {
			if change_recieved() { // semua yang baunya recieving di leader kayaknya harus beda thread
				change_recieved = false;
				send_log_order();
			} else if change_confirmed() { // kayaknya change_confirmed harus beda thread
				change_confirmed = false;
				send_log_confirmation();
			} else {
				send_append_entries();
			}
			wait_until_heartbeat();
		}
	}
}