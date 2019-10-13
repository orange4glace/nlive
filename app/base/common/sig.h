#ifndef NLIVE_SIG_H_
#define NLIVE_SIG_H_

#include <boost/signals2/signal.hpp>
#include <boost/shared_ptr.hpp>

#define sig2_t boost::signals2::signal
#define sig2_conn_t boost::signals2::connection

#define SIG2_TRACK(a) (a).track(__sig_scope_)

namespace nlive {

namespace {

struct Scope {};

}

class Sig {

protected:
  boost::shared_ptr<Scope> __sig_scope_;

  inline Sig() :
    __sig_scope_(new Scope()) {
  }

};

}

#endif