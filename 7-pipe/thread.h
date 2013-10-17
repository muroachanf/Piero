
class thread{
    private:
        HANDLE hThread;
        DWORD threadID;
        HANDLE hMutex ;
    public :
        ~thread();
        void run();
        virtual void proc();
        void notify_exit();
        BOOL should_exit();
        BOOL state_mutex();
        BOOL query_state_mutex();
        void close_state_mutex();
};

