#ifndef __CONNECTSEGMENT_H__
#define __CONNECTSEGMENT_H__


#include <vector>
#include "../tool/Map.h"
#include "../paras/Match.h"

using std::vector;

static const size_t MAX_LINKING_OVERLAP = 10;


template<typename _val>
double connectSegment(Segment<_val> &h1, Segment<_val> &h2)
{
    if(h1.strand() == h2.strand() && h2.top_evalue_ == -1) 
    {
		Segment<_val> *p = &h1;
		while(p != 0) 
        {
			if(p->query_range().overlap(h2.query_range())/3 >= 1
				|| p->subject_range().overlap(h2.subject_range()) > MAX_LINKING_OVERLAP)
				return std::numeric_limits<double>::max();
			p = p->next_;
		}
		double ev = h1.evalue_ * h2.evalue_;
		h2.next_ = h1.next_;
		h1.next_ = &h2;
		p = &h1;
		while(p != 0) {
			p->evalue_ = ev;
			p->top_evalue_ = ev;
			p = p->next_;
		}
		return ev;
	}
	return std::numeric_limits<double>::max();

}


template<typename _val>
void connectSegment(const typename vector<Segment<_val> >::iterator &begin, const typename vector<Segment<_val> >::iterator &end)
{
    int max_score = begin->score_;
	for(typename vector<Segment<_val> >::iterator i=begin; i<end; ++i)
		i->top_score_ = max_score;

}

template<typename _val>
void connectSegment(vector<Segment<_val> > &hsp_list)
{
    typedef Map<typename vector<Segment<_val> >::iterator,typename Segment<_val>::Subject> Hsp_map;
	std::sort(hsp_list.begin(), hsp_list.end(), Segment<_val>::compSubject);
	Hsp_map hsp_map (hsp_list.begin(), hsp_list.end());
	typename Hsp_map::Iterator it = hsp_map.begin();
	while(it.valid()) 
    {
		connectSegment<_val>(it.begin(), it.end());
		++it;
	}

}






#endif // __CONNECTSEGMENT_H__