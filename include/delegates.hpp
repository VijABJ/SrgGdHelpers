#pragma once
#ifndef __CREAKY_DELEGATES_T_H__
#define __CREAKY_DELEGATES_T_H__

#include <vector>
#include <map>
#include <memory>
#include <3rdParty/fast_delegates/FastDelegateWrapper.h>

// uncomment the following if you want empty items in the map to be removed
// this is used by the mapped_delegates detach() member
//#define YAGLIB_DELEGATES_ERASE_EMPTY

namespace creaky
{

    /**
     * this symbol controls how many arguments are supported by the template
     * declaration/generation for delegates.  however, it cannot go beyond 8
     * right now because it is a hard-coded cap inside FastDelegate.h
     */
    constexpr int YAGLIB_DELEGATES_MAX_ARGUMENTS_SUPPORTED = 8;

    /**
     * all delegates exposes the following functionalities:
     * -------------------------------------------------------------------------
     * size()
     *  indicates how many delegates, or in some cases, group of delegates are
     *  currently contained.
     *
     * attach(func), attach(method, object)
     *  installs either a function/functor or a function object.  if it is a
     *  function object, the reference to the object to be invoked MUST be
     *  provided.
     *
     * detach(func), detach(method, object)
     *  removes the previously installed handler.  note that there is a version
     *  that also accepts the function object along with the object reference.
     *  this is done in order to be able to find the handler in the list.
     *  internally, fastdelegate objects are created, but to do comparison, the
     *  original values used during attach must be provided.
     *
     * operator()
     *  various versions of operator() are declared for whatever number of
     *  arguments triggered the template expansion.  other delegate variations
     *  may have extra arguments required however.
     *
     */

     /**
      * basic_delegates
      * ---------------
      *
      * this is the basic delegates container.  all it does is manage the
      * handlers, and forwards calls to it unconditionally.
      *
      */
    template<typename... Args>
    class basic_delegates
    {
        static_assert(sizeof...(Args) <= YAGLIB_DELEGATES_MAX_ARGUMENTS_SUPPORTED, "Templated function has too many parameters");

    public:
        typedef typename fastdelegate::FastDelegate<void(Args...)> callback_t;

        void operator()(Args... args)
        {
            for (auto& cb : callbacks)
                cb(args...);
        }

        size_t size() const { return callbacks.size(); }
        void clear() { callbacks.clear(); };

        template<typename T>
        void attach(T t)
        {
            if (!exists(t))
                callbacks.push_back(callback_t(t));
        }

        template<typename T, typename C>
        void attach(T t, C c)
        {
            if (!exists(t, c))
                callbacks.push_back(cbx(t, c));
        }

        template<typename T>
        void detach(T t)
        {
            auto it = find(callback_t(t));
            if (it != callbacks.end())
                callbacks.erase(it);
        }

        template<typename T, typename C>
        void detach(T t, C c)
        {
            auto it = find(cbx(t, c));
            if (it != callbacks.end())
                callbacks.erase(it);
        }

    protected:
        typedef std::vector<callback_t> callback_list_t;
        callback_list_t callbacks;

        template<typename T, typename C>
        callback_t cbx(T t, C c)
        {
            callback_t cb;
            cb.bind(c, t);
            return cb;
        }

        template<typename T>
        inline bool exists(T t)
        {
            return find(t) != callbacks.end();
        }

        template<typename T, typename C>
        inline bool exists(T t, C c)
        {
            return find(t, c) != callbacks.end();
        }

        template<typename T>
        inline auto find(T t)
        {
            return find(callback_t(t));
        }

        template<typename T, typename C>
        inline auto find(T t, C c)
        {
            return find(cbx(t, c));
        }

        inline auto find(const callback_t& cb)
        {
            return std::find(callbacks.begin(), callbacks.end(), cb);
        }
    };


    /**
     * parametric_delegates
     * --------------------
     *
     * this is a variation of basic_delegates.  the major difference is that this one
     * is meant to be inherited from instead of as-is instantiation.  when forwarding,
     * canForward() is first called to confirm whether the handler will in fact be
     * called.  the attach() members also accept an extra parameter of type extra_data_t
     * (supplied during template instantiation).  this extra parameter is also stored
     * and can be used later to determine whether or not to allow forwarding.
     *
     * this could be useful in implementing delegates that depends in some way on the
     * parameters being passed in, or in the internal state of the object caller.  do NOT
     * use this if you intend to map events to some storable value, say like an interrupt
     * handler style event list.  if that is what you want, use mapped_delegates instead.
     *
     */
    template<typename extra_data_t, typename... Args>
    class parametric_delegates {
    public:
        typedef typename fastdelegate::FastDelegate<void(Args... args)> callback_t;

        void operator()(Args... args)
        {
            for (auto& cb : callbacks)
                if (canForward(cb.extra, args...))
                    (cb.callback)(args...);
        }

        size_t size() const { return callbacks.size(); }
        void clear() { callbacks.clear(); };

        template<typename T>
        void attach(const extra_data_t extra, T t)
        {
            if (!exists(t))
                callbacks.push_back(inner_data_t(extra, callback_t(t)));
        }

        template<typename T, typename C>
        void attach(const extra_data_t extra, T t, C c)
        {
            if (!exists(t, c))
                callbacks.push_back(inner_data_t(extra, cbx(t, c)));
        }

        template<typename T>
        void detach(T t)
        {
            auto it = find(t);
            if (it != callbacks.end())
                callbacks.erase(it);
        }

        template<typename T, typename C>
        void detach(T t, C c)
        {
            auto it = find(t, c);
            if (it != callbacks.end())
                callbacks.erase(it);
        }

    protected:
        struct inner_data_t
        {
            extra_data_t extra;
            callback_t callback;
            inner_data_t(extra_data_t _extra, callback_t _callback) : extra(_extra), callback(_callback) {};
        };

        using callback_list_t = std::vector<inner_data_t> ;
        callback_list_t callbacks;

        template<typename T, typename C>
        callback_t cbx(T t, C c)
        {
            callback_t cb;
            cb.bind(c, t);
            return cb;
        }

        template<typename T>
        inline bool exists(T t)
        {
            return find(t) != callbacks.end();
        }

        template<typename T, typename C>
        inline bool exists(T t, C c)
        {
            return find(t, c) != callbacks.end();
        }

        template<typename T>
        inline auto find(T t)
        {
            return find(callback_t(t));
        }

        template<typename T, typename C>
        inline auto find(T t, C c)
        {
            return find(cbx(t, c));
        }

        inline auto find(const callback_t& cb)
        {
            return std::find_if(callbacks.begin(), callbacks.end(), [&](auto& item) { return item.callback == cb; });
        }

        virtual bool canForward(const extra_data_t, Args...) { return true; };
    };


    /**
     * mapped_delegates
     * ----------------
     *
     * this is meant to be used for interrupt-table style dispatching.  internally,
     * there is a hash map container that matches option_t values with a basic_delegate.
     * sub-delegates stored are wrapped around shared_ptr in order to obviate
     * the need for a destructor.
     *
     * aside from the delegate-standard operator(), a notify_all() method is also
     * provided to forward calls to ALL the contained handlers.
     *
     */
    template<typename option_t, typename... Args>
    class mapped_delegates
    {
    public:
        void notify_all(Args... args)
        {
            for (auto& cb : callbacks)
                (cb.second)(args...);
        }

        void operator()(option_t opt, Args... args)
        {
            auto it = callbacks.find(opt);
            if (it != callbacks.end())
                (*(it->second))(args...);
        }

        size_t size() const { return callbacks.size(); }
        void clear() { callbacks.clear(); };

        template<typename T>
        void attach(option_t opt, T t)
        {
            auto target = get(opt);
            target->second->attach(t);
        }

        template<typename T, typename C>
        void attach(option_t opt, T t, C c)
        {
            auto target = get(opt);
            target->second->attach(t, c);
        }

        template<typename T>
        void detach(option_t opt, T t)
        {
            auto target = callbacks.find(opt);
            if (target != callbacks.end())
            {
                target->second->detach(t);
#ifdef YAGLIB_DELEGATES_ERASE_EMPTY
                if (target->second->size() == 0)
                    callbacks.erase(target);
#endif // YAGLIB_DELEGATES_ERASE_EMPTY  
            }
        }

        template<typename T, typename C>
        void detach(option_t opt, T t, C c)
        {
            auto target = callbacks.find(opt);
            if (target != callbacks.end())
            {
                target->second->detach(t, c);
#ifdef YAGLIB_DELEGATES_ERASE_EMPTY
                if (target->second->size() == 0)
                    callbacks.erase(target);
#endif // YAGLIB_DELEGATES_ERASE_EMPTY  
            }
        }

    protected:
        typedef basic_delegates<Args...> inner_delegates_t;
        typedef std::shared_ptr<inner_delegates_t> p_inner_delegates_t;
        typedef std::map<option_t, p_inner_delegates_t> callback_list_t;

        callback_list_t callbacks;

        // this works as find(), except that when it does not find opt, it creates it and return that
        auto get(option_t opt)
        {
            auto target = callbacks.find(opt);
            if (target == callbacks.end())
            {
                callbacks[opt] = p_inner_delegates_t(new inner_delegates_t());
                target = callbacks.find(opt);
            }
            return target;
        }

    };

} /// namespace creaky

#endif /// __CREAKY_DELEGATES_T_H__

