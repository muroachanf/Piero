class cmd;
typedef void (*data_notify_proc)(char *d);
cmd* cmd_init(data_notify_proc proc);
void cmd_run(cmd * c ,const char *buff);
void cmd_free(cmd *c);