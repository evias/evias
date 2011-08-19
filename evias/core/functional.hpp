#ifndef __EVIAS_CORE_FUNCTIONAL__
#define __EVIAS_CORE_FUNCTIONAL__

#include <exception>
#include <string>
#include <vector>
#include <queue>
#include <map>

#include "assertable.hpp"

namespace evias {

namespace core {

    using std::vector;
    using std::queue;
    using std::string;
    using std::pair;

    /**
     * @brief
     * - evias::core::callback
     * - evias::core::callgroup
     *
     * callback :
     * interface for defining objects pointing to function
     * pointers. This allows an easier and more C++'er
     * way to execute the callback functions. Also it
     * is very flexible and allows any kind of 2 parameters
     * functions.
     *
     * callgroup :
     * container for callback methods with parameters vector pairs.
     * This means the first entry of the pair will be the evias::core::callback
     * object and the second entry will be a vector<_cg_param_t>
     * with _cg_param_t = callback function pointer parameters type
     * This "pair" is easier described as the actual "call" to the
     * function pointer, as we store the parameters for the function
     * call and the actual function.
     *
     * @todo implement class member function pointers
     **/
    template <typename _cb_return_t,
              typename _cb_param_t,
              typename _cb_funcptr_t>
    class callback
    {
        typedef _cb_return_t (*__my_decl) (_cb_param_t, _cb_param_t);

        _cb_funcptr_t m_funcptr;

    public :
        callback(_cb_funcptr_t=NULL);
        callback(const callback&);

        void set_funcptr(_cb_funcptr_t);

        virtual ~callback() {};

        _cb_return_t execute(_cb_param_t, _cb_param_t);
    };

    template <typename _cg_return_t,
              typename _cg_param_t,
              typename _cg_funcptr_t>
    class callqueue
    {
        typedef _cg_return_t (*__my_decl) (_cg_param_t, _cg_param_t);

        // the callbacks which have been pushed to the callqueue instance
        typedef queue<callback<_cg_return_t,_cg_param_t,_cg_funcptr_t>*> __callbackQueue;

        __callbackQueue             m_callbacks;
        queue<vector<_cg_param_t> > m_params;

        vector<_cg_return_t>        m_lastReturns;

    public :
        callqueue();

        ~callqueue();

        /** push **/
        void push (callback<_cg_return_t,_cg_param_t,_cg_funcptr_t>*, _cg_param_t, _cg_param_t);

        /** pop **/
        typedef std::pair<callback<_cg_return_t, _cg_param_t, _cg_funcptr_t>*, vector<_cg_param_t> > __call;
        __call pop ();

        /** run **/
        vector<_cg_return_t> run();
    };

    // IMPLEMENTATION
    // class callback

    /**
     * @brief
     * constructor, assign function pointer (or null if empty)
     *
     * @param   cb  _cb_funcptr_t   The function pointer of type _cb_funcptr_t (3rd template)
     **/
    template <typename _cb_return_t,
              typename _cb_param_t,
              typename _cb_funcptr_t>
    callback<_cb_return_t, _cb_param_t, _cb_funcptr_t>::callback(_cb_funcptr_t cb)
        : m_funcptr(cb)
    {
    }

    /**
     * @brief
     * copy constructor
     *
     * @param   r   const callback&     The object you want to clone
     **/
    template <typename _cb_return_t,
              typename _cb_param_t,
              typename _cb_funcptr_t>
    callback<_cb_return_t, _cb_param_t, _cb_funcptr_t>::callback(const callback& r)
        : m_funcptr(r.m_funcptr)
    {
    }

    /**
     * @brief
     * execute the callback function. The function pointer has to point
     * to a valid 2 parameters (of same type) function defined before.
     *
     * @param   p1  _cb_param_t     First parameter for the call
     * @param   p2  _cb_param_t     Second parameter for the call
     *
     * @return  _cb_return_t    The return result of the function pointer call
     **/
    template <typename _cb_return_t,
              typename _cb_param_t,
              typename _cb_funcptr_t>
    _cb_return_t callback<_cb_return_t, _cb_param_t, _cb_funcptr_t>::execute(_cb_param_t p1, _cb_param_t p2)
    {
        return (*m_funcptr)(p1, p2);
    }

    /**
     * @brief
     * assign (new) function pointer
     *
     * @param   fptr    _cb_funcptr_t   The function pointer..
     **/
    template <typename _cb_return_t,
              typename _cb_param_t,
              typename _cb_funcptr_t>
    void callback<_cb_return_t, _cb_param_t, _cb_funcptr_t>::set_funcptr(_cb_funcptr_t fptr)
    {
        m_funcptr = fptr;
    }

    // IMPLEMENTATION
    // class callqueue

    /**
     * @brief
     * constructor
     **/
    template <typename _cg_return_t,
              typename _cg_param_t,
              typename _cg_funcptr_t>
    callqueue<_cg_return_t, _cg_param_t, _cg_funcptr_t>::callqueue()
    {
    }

    /**
     * @brief
     * destructor, cleans all available vectors & queues..
     **/
    template <typename _cg_return_t,
              typename _cg_param_t,
              typename _cg_funcptr_t>
    callqueue<_cg_return_t, _cg_param_t, _cg_funcptr_t>::~callqueue()
    {
        m_lastReturns.clear();

        // empty queues
        while (! m_callbacks.empty()) m_callbacks.pop();
        while (! m_params.empty()) m_params.pop();
    }

    /**
     * @brief
     * push allows to append a call to the call queue. The queue works
     * in a FIFO strategy so the first pushed call configuration will
     * be executed the first as well.
     *
     * @param   cb  callback<T1,T2,T3>      the callback object to push on the queue
     *                  [With T1=_cg_return_t, T2=_cg_param_t, T3=_cg_funcptr_t]
     * @param   p1  _cg_param_t             first parameter to the function call
     * @param   p2  _cg_param_t             second parameter to the function call
     **/
    template <typename _cg_return_t,
              typename _cg_param_t,
              typename _cg_funcptr_t>
    void callqueue<_cg_return_t, _cg_param_t, _cg_funcptr_t>::push(callback<_cg_return_t, _cg_param_t, _cg_funcptr_t>* cb, _cg_param_t p1, _cg_param_t p2)
    {
        // build parameters vector
        _cg_param_t p[2] = {p1,p2};
        vector<_cg_param_t> args(p, p+2);

        // store
        m_callbacks.push(cb);
        m_params.push(args);
    }

    /**
     * @brief
     * pop removes the first entry of the queues. Basically it allows you
     * to get the "callee" which you can consider to be the actual "call"
     * configuration of your first queue entry.
     *
     * @return  pair<callback<T1,T2,T3>*, vector<T2>>   the callee configuration
     *              [With T1=_cg_return_t,T2=_cg_param_t,T3=_cg_funcptr_t]
     **/
    template <typename _cg_return_t,
              typename _cg_param_t,
              typename _cg_funcptr_t>
    std::pair<callback<_cg_return_t,_cg_param_t,_cg_funcptr_t>*, vector<_cg_param_t> >

        callqueue<_cg_return_t,_cg_param_t,_cg_funcptr_t>::pop()
    {
        // pop first inserted callback object and first insert params vector
        callback<_cg_return_t,_cg_param_t,_cg_funcptr_t>* cb = m_callbacks.front();
        vector<_cg_param_t> params = m_params.front();

        m_callbacks.pop();
        m_params.pop();

        // return a combined std::pair with the callback object and the params std::vector
        typedef std::pair<callback<_cg_return_t,_cg_param_t,_cg_funcptr_t>*, vector<_cg_param_t> > __my_call;
        return __my_call(cb, params);
    }

    /**
     * @brief
     * run the queue's content.
     *
     * @return vector<_cg_return_t>     The list of return results.
     **/
    template <typename _cg_return_t,
              typename _cg_param_t,
              typename _cg_funcptr_t>
    vector<_cg_return_t> callqueue<_cg_return_t, _cg_param_t, _cg_funcptr_t>::run()
    {
        typedef std::pair<callback<_cg_return_t,_cg_param_t,_cg_funcptr_t>*, vector<_cg_param_t> > __my_call;

        m_lastReturns.clear();

        while (! m_callbacks.empty()) {

            // call callgroup::pop which will pop both queues and
            // return a combined result (@see typedef __my_call)
            __my_call current = pop();

            vector<_cg_param_t> params = current.second;
            _cg_return_t r = (current.first)->execute(params[0], params[1]);

            m_lastReturns.push_back(r);
        }

        return m_lastReturns;
    }

}; // end namespace core

}; // end namespace evias

#endif
