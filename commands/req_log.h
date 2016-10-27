//
//  req_log.h
//  c-ipfs
//
//  Created by John Jones on 10/27/16.
//  Copyright © 2016 JMJAtlanta. All rights reserved.
//

#ifndef __COMMANDS_REQ_LOG_H__
#define __COMMANDS_REQ_LOG_H__

struct ReqLogEntry {
	long start_time;
	long end_time;
	int active;
	char* command;
	struct Option** options;
	struct Argument** args;
	int id;
	struct Request* req;
	struct ReqLog* log;
};

struct ReqLog {
	struct ReqLogEntry** requests;
	int next_id;
	//mutex lock;
	long keep_duration;
};

#endif /* req_log_h */
