#ifndef __CONSTPARAMETER_H__
#define __CONSTPARAMETER_H__


class ConstParameter
{
    public:
    enum
    {
        build_version = 1,
        build_compatibility = 1,
        vat_version = 0,
        seedp_bits = 10,
        seedp = 1 << seedp_bits,
        max_seed_weight = 32,
        seqp_bits = 8,
        seqp = 1 << seqp_bits,
        max_shapes = 16,
        index_modes = 2,
        min_shape_len = 10,
        max_shape_len = 32,
        seed_anchor = 8

    };
    static const char *version_string;
    static const char *program_name;
    static const char *id_delimiters;
};




#endif // __CONSTPARAMETER_H__