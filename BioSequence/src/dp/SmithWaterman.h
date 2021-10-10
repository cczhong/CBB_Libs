#ifndef __SMITH_WATERMAN_H__
#define __SMITH_WATERMAN_H__
#include "../sequence/Sequence.h"
#include "../paras/statistics.h"
#include "ScoreProfile.h"
#include "DPMatrix.h"

template<typename _score>
inline ScoreVector<_score> cell_update(const ScoreVector<_score> &diagonal_cell,
						 const ScoreVector<_score> &scores,
						 const ScoreVector<_score> &gap_extension,
						 const ScoreVector<_score> &gap_open,
						 ScoreVector<_score> &horizontal_gap,
						 ScoreVector<_score> &vertical_gap,
						 ScoreVector<_score> &best,
						 const ScoreVector<_score> &vbias)
{
	ScoreVector<_score> current_cell = diagonal_cell + scores;
	current_cell.unbias(vbias);
	current_cell.max(vertical_gap).max(horizontal_gap);
	best.max(current_cell);
	vertical_gap -= gap_extension;
	horizontal_gap -=  gap_extension;
	ScoreVector<_score> open = current_cell - gap_open;
	vertical_gap.max(open);
	horizontal_gap.max(open);
	return current_cell;
}


template <typename _val, typename _score, typename _callback>
void smithWaterman(const Sequence<const _val> &query,
                        const vector<Sequence<const _val> > &subjects,
                        unsigned band,
                        unsigned padding,
                        int op,
                        int ep,
                        int filter_score,
                        _callback &f,
                        const _score &,
                        Statistics &stats)
{

							typedef ScoreVector<_score> sv;
							unsigned qlen(query.length());
							unsigned slen(subjects[0].length());
							DPMatrix<_score> dp(slen, qlen, band, padding);

							sv open_penalty(static_cast<char>(op));
							sv extend_penalty(static_cast<char>(ep));
							//bias
							sv vbias(0);

							SequenceStream dseq;
							ScoreProfile<_score> profile;

							typename vector<Sequence<const _val> >::const_iterator subject_it(subjects.begin());
							while (subject_it < subjects.end())
							{

								const unsigned n_subject(std::min((unsigned)ScoreTraits<_score>::channels, (unsigned)(subjects.end() - subject_it)));
								typename vector<Sequence<const _val> >::const_iterator subject_end(subject_it + n_subject);
								sv best;
								dseq.reset();
								dp.clear();

								for (unsigned j = 0; j < slen; ++j)
								{
									typename DPMatrix<_score>::ColumnIterator it(dp.begin(j));
									sv vgap, hgap, column_best;
									profile.template SetValue<_val>(dseq.get<_val>(subject_it, subject_end, j, _score()));

									while (!it.at_end())
									{
										hgap = it.hgap();
										sv next = cell_update<_score>(it.diag(), profile.get(query[it.row_pos_]), extend_penalty, open_penalty, hgap, vgap, column_best, vbias);
										it.set_hgap(hgap);
										it.set_score(next);
										++it;
									}
									best.max(column_best);
								}

								for (unsigned i = 0; i < n_subject; ++i)
									if (best[i] >= filter_score)
										f(i + (subject_it - subjects.begin()), *(subject_it + i), best[i]);
								subject_it += ScoreTraits<_score>::channels;
	}
							//cout << "Smith_Waterman end" << endl;

}



#endif // __SMITH_WATERMAN_H__