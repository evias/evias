#ifndef __EVIAS_CORE_FUNCTIONAL__
#define __EVIAS_CORE_FUNCTIONAL__

#include <exception>
#include <string>
#include <vector>
#include <queue>

#include "assertable.hpp"

namespace evias {

namespace core {

    using std::vector;
    using std::queue;
    using std::string;

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
     *
     * @todo implement class member function pointers
     *
     * @example
     * bool intEquals(int i1, int i2)
     * {
     *     return i1 == i2;
     * }
     *
     * typedef bool (*my_callback_t) (int, int);
     * evias::core::callback<bool, int, my_callback_t>* f = new evias::core::callback<bool, int, my_callback_t>();
     *
     * my_callback_t cb = NULL;
     * cb = &intEquals;
     * f->setCallback(cb);
     *
     * bool r1 = f->execute(1, 1); // return (bool) true
     * bool r2 = f->execute(1, 2); // return (bool) false
     * bool r3 = f->execute(42, 42); // return (bool) true
     * bool r4 = f->execute(42, 24); // return (bool) false, sadly
     *
     * OR
     * typedef bool (*my_callback_t) (int, int);
     * evias::core::callback<bool, int, my_callback_t>* cb_instance = new evias::core::callback<bool, int, my_callback_t>(&intEquals);
     * evias::core::callgroup<bool,int,my_callback_t>* g = new callgroup<bool,int,my_callback_t>();
     *
     * g->push(cb_instance, 1, 1);
     * g->push(cb_instance, 1, 2);
     * g->push(cb_instance, 42, 42);
     * g->push(cb_instance, 42, 42);
     *
     * vector<bool> returns = g->run();
     **/
    template <typename _cb_return_t, typename _cb_param_t, typename _cb_funcptr_t>
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

    template <typename _cg_return_t, typename _cg_param_t, typename _cg_funcptr_t>
    class callgroup
    {
        typedef _cg_return_t (*__my_decl) (_cg_param_t, _cg_param_t);
        typedef queue<callback<_cg_return_t,_cg_param_t,_cg_funcptr_t>*> __callbackQueue;

        __callbackQueue             m_callbacks;
        queue<vector<_cg_param_t> > m_params;

        vector<_cg_return_t>        m_lastReturns;

    public :
        callgroup();

        ~callgroup();

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

    template <typename _cb_return_t, typename _cb_param_t, typename _cb_funcptr_t>
    callback<_cb_return_t, _cb_param_t, _cb_funcptr_t>::callback(_cb_funcptr_t cb)
        : m_funcptr(cb)
    {
    }

    template <typename _cb_return_t, typename _cb_param_t, typename _cb_funcptr_t>
    callback<_cb_return_t, _cb_param_t, _cb_funcptr_t>::callback(const callback& r)
        : m_funcptr(r.m_funcptr)
    {
    }

    template <typename _cb_return_t, typename _cb_param_t, typename _cb_funcptr_t>
    _cb_return_t callback<_cb_return_t, _cb_param_t, _cb_funcptr_t>::execute(_cb_param_t p1, _cb_param_t p2)
    {
        return (*m_funcptr)(p1, p2);
    }

    template <typename _cb_return_t, typename _cb_param_t, typename _cb_funcptr_t>
    void callback<_cb_return_t, _cb_param_t, _cb_funcptr_t>::set_funcptr(_cb_funcptr_t fptr)
    {
        m_funcptr = fptr;
    }

    // IMPLEMENTATION
    // class callgroup

    template <typename _cg_return_t, typename _cg_param_t, typename _cg_funcptr_t>
    callgroup<_cg_return_t, _cg_param_t, _cg_funcptr_t>::callgroup()
    {
    }

    template <typename _cg_return_t, typename _cg_param_t, typename _cg_funcptr_t>
    callgroup<_cg_return_t, _cg_param_t, _cg_funcptr_t>::~callgroup()
    {
        m_lastReturns.clear();

        // empty queues
        while (! m_callbacks.empty()) m_callbacks.pop();
        while (! m_params.empty()) m_params.pop();
    }

    template <typename _cg_return_t, typename _cg_param_t, typename _cg_funcptr_t>
    void callgroup<_cg_return_t, _cg_param_t, _cg_funcptr_t>::push(callback<_cg_return_t, _cg_param_t, _cg_funcptr_t>* cb, _cg_param_t p1, _cg_param_t p2)
    {
        // build parameters vector
        _cg_param_t p[2] = {p1,p2};
        vector<_cg_param_t> args(p, p+2);

        // store
        m_callbacks.push(cb);
        m_params.push(args);
    }

    template <typename _cg_return_t, typename _cg_param_t, typename _cg_funcptr_t>
    std::pair<callback<_cg_return_t,_cg_param_t,_cg_funcptr_t>*, vector<_cg_param_t> >
        callgroup<_cg_return_t,_cg_param_t,_cg_funcptr_t>::pop()
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

    template <typename _cg_return_t, typename _cg_param_t, typename _cg_funcptr_t>
    vector<_cg_return_t> callgroup<_cg_return_t, _cg_param_t, _cg_funcptr_t>::run()
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
