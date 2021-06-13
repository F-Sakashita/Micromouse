#ifndef TASK_TEMPLATE_HPP
#define TASK_TEMPLATE_HPP

template <typename OsFunc_t>
class TaskTemplate{
    public:
        TaskTemplate(){
            bInitialized = false;
        }
        ~TaskTemplate(){;}

        //初期化関数(派生クラスで定義すること)
        virtual bool Initialize(const OsFunc_t *pOsFunc) = 0;
        //更新関数(派生クラスで定義すること)
        virtual void Update() = 0;

        bool IsInitialize(){
            return bInitialized;
        }

        bool bInitialized;  //初期化完了  
        OsFunc_t *pOsFunc; 
    private:
        

    protected:

};


#endif /* TASK_TEMPLATE_HPP */