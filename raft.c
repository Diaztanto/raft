void raft_activity() {
	while (1) {
		if follower() {
			wait_until_receive_or_timeout();
			if recieve() {
				if (recieved->type = LOG_ORDER) {
					add_log(recieved);
					if (leader == null) {
						leader = recieved->sender;
					}
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
				} else if (recieved->type = LOG_ORDER) {
					declare_self_follower();
					add_log(recieved);
					leader = recieved->sender;
				} else {
					throw_exception();
				}
			}
		} else if leader() {

		}
	}
}