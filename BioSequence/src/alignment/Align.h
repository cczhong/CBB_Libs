#ifndef __ALIGN_H__
#define __ALIGN_H__

#include "../paras/statistics.h"
#include "FilterHit.h"
#include "XdropUngapped.h"
#include "../paras/ShapeConfigure.h"

template <typename _val, typename _locr, typename _locq, typename _locl>
                                    void hitAlign(const _locq q_pos,
                                                const _val *query,
                                                _locr s,
                                                Statistics &stats,
                                                const unsigned sid,
                                                HitFilter<_val, _locr, _locq, _locl> &hf)
{
    stats.inc(Statistics::TENTATIVE_MATCHES0);
    const _val *subject = ReferenceSeq<_val>::data_->data(s);
    unsigned  length, delta;
    int score = xdropUngapped<_val, _locr, _locq>(query, subject, ShapeConfigure::get().getShape(sid).length_, delta, length);
    if (score < 0)
        return;
    stats.inc(Statistics::TENTATIVE_MATCHES2);
    hf.push(s,score);

}
#endif // __ALIGN_H__