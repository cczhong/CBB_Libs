#ifndef __COLLISION_H__
#define __COLLISION_H__

template<typename _val, typename _pos>
class Collision
{
    public:
        bool PrimaryHit(const _val *query,
                            const _val *subject,
                            const unsigned seed_offset,
                            const unsigned sid,
                            const unsigned len);

};

#endif // __COLLISION_H__