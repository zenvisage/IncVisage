#if 0
#include <cstdio>
#include <vector>
#include <string>
#include <utility>

#include "eugenewu.h"
#include "utils.h"

using namespace std;

const int NUM_G0 = 10;
const int NUM_G1 = 10;
const int NUM_G2 = 20;

void base_query0(SwiftIndex& swi)
{
    vector<SwiftIndex::constraintlist*> constraints;
    vector<double> means;
    vector<string> target_names;
    target_names.push_back("a0");
    target_names.push_back("a1");

    for (int g0 = 0; g0 < NUM_G0; g0++) {
        for (int g1 = 0; g1 < NUM_G1; g1++) {
            for (int g2 = 0; g2 < NUM_G2; g2++) {
                constraints.clear();
                SwiftIndex::constraintlist c0;
                c0.push_back(make_pair("g0", (uint8_t*) &g0));
                constraints.push_back(&c0);
                SwiftIndex::constraintlist c1;
                c1.push_back(make_pair("g1", (uint8_t*) &g1));
                constraints.push_back(&c1);
                SwiftIndex::constraintlist c2;
                c2.push_back(make_pair("g2", (uint8_t*) &g2));
                constraints.push_back(&c2);

                swi.multi_ordered_sample_mean(target_names, means, constraints,
                        20000);
            }
        }
    }
}

void base_query1(SwiftIndex& swi)
{
    vector<SwiftIndex::constraintlist*> constraints;
    vector<double> means;
    vector<string> target_names;
    target_names.push_back("a0");
    target_names.push_back("a1");

    for (int g0 = 0; g0 < NUM_G0; g0++) {
        for (int g1 = 0; g1 < NUM_G1; g1++) {
            constraints.clear();
            SwiftIndex::constraintlist c0;
            c0.push_back(make_pair("g0", (uint8_t*) &g0));
            constraints.push_back(&c0);
            SwiftIndex::constraintlist c1;
            c1.push_back(make_pair("g1", (uint8_t*) &g1));
            constraints.push_back(&c1);

            swi.multi_ordered_sample_mean(target_names, means, constraints,
                    20000);
        }
    }
}

void base_query2(SwiftIndex& swi)
{
    vector<SwiftIndex::constraintlist*> constraints;
    vector<double> means;
    vector<string> target_names;
    target_names.push_back("a0");
    target_names.push_back("a1");

    for (int g0 = 0; g0 < NUM_G0; g0++) {
        constraints.clear();
        SwiftIndex::constraintlist c0;
        c0.push_back(make_pair("g0", (uint8_t*) &g0));
        constraints.push_back(&c0);

        swi.multi_ordered_sample_mean(target_names, means, constraints, 20000);
    }
}

void val_filter0(SwiftIndex& swi)
{
    vector<SwiftIndex::constraintlist*> constraints;
    vector<double> means;
    vector<string> target_names;
    target_names.push_back("a0");
    target_names.push_back("a1");

    for (int g0 = 0; g0 < NUM_G0; g0++) {
        for (int g1 = 0; g1 < NUM_G1; g1++) {
            for (int g2 = 0; g2 < NUM_G2; g2++) {
                constraints.clear();
                SwiftIndex::constraintlist c0;
                c0.push_back(make_pair("g0", (uint8_t*) &g0));
                constraints.push_back(&c0);
                SwiftIndex::constraintlist c1;
                c1.push_back(make_pair("g1", (uint8_t*) &g1));
                constraints.push_back(&c1);
                SwiftIndex::constraintlist c2;
                c2.push_back(make_pair("g2", (uint8_t*) &g2));
                constraints.push_back(&c2);
                SwiftIndex::constraintlist c3;
                int a2 = 85;
                c3.push_back(make_pair("a2", (uint8_t*) &a2));
                constraints.push_back(&c3);

                swi.multi_ordered_sample_mean(target_names, means, constraints,
                        20000);
            }
        }
    }
}
void val_filter1(SwiftIndex& swi)
{
    vector<SwiftIndex::constraintlist*> constraints;
    vector<double> means;
    vector<string> target_names;
    target_names.push_back("a0");
    target_names.push_back("a1");

    for (int g0 = 0; g0 < NUM_G0; g0++) {
        for (int g1 = 0; g1 < NUM_G1; g1++) {
            constraints.clear();
            SwiftIndex::constraintlist c0;
            c0.push_back(make_pair("g0", (uint8_t*) &g0));
            constraints.push_back(&c0);
            SwiftIndex::constraintlist c1;
            c1.push_back(make_pair("g1", (uint8_t*) &g1));
            constraints.push_back(&c1);
            SwiftIndex::constraintlist c3;
            int a2 = 85;
            c3.push_back(make_pair("a2", (uint8_t*) &a2));
            constraints.push_back(&c3);

            swi.multi_ordered_sample_mean(target_names, means, constraints,
                    20000);
        }
    }
}
void val_filter2(SwiftIndex& swi)
{
    vector<SwiftIndex::constraintlist*> constraints;
    vector<double> means;
    vector<string> target_names;
    target_names.push_back("a0");
    target_names.push_back("a1");

    for (int g0 = 0; g0 < NUM_G0; g0++) {
        constraints.clear();
        SwiftIndex::constraintlist c0;
        c0.push_back(make_pair("g0", (uint8_t*) &g0));
        SwiftIndex::constraintlist c3;
        int a2 = 85;
        c3.push_back(make_pair("a2", (uint8_t*) &a2));
        constraints.push_back(&c3);

        swi.multi_ordered_sample_mean(target_names, means, constraints,
                20000);
    }
}
void val_filter3(SwiftIndex& swi)
{
    vector<SwiftIndex::constraintlist*> constraints;
    vector<double> means;
    vector<string> target_names;
    target_names.push_back("a0");
    target_names.push_back("a1");

    for (int g0 = 0; g0 < NUM_G0; g0++) {
        for (int g1 = 0; g1 < NUM_G1; g1++) {
            for (int g2 = 0; g2 < NUM_G2; g2++) {
                constraints.clear();
                SwiftIndex::constraintlist c0;
                c0.push_back(make_pair("g0", (uint8_t*) &g0));
                constraints.push_back(&c0);
                SwiftIndex::constraintlist c1;
                c1.push_back(make_pair("g1", (uint8_t*) &g1));
                constraints.push_back(&c1);
                SwiftIndex::constraintlist c2;
                c2.push_back(make_pair("g2", (uint8_t*) &g2));
                constraints.push_back(&c2);
                SwiftIndex::constraintlist c3;
                int a20 = 85;
                int a21 = 65;
                c3.push_back(make_pair("a2", (uint8_t*) &a20));
                c3.push_back(make_pair("a2", (uint8_t*) &a21));
                constraints.push_back(&c3);

                swi.multi_ordered_sample_mean(target_names, means, constraints,
                        20000);
            }
        }
    }
}
void val_filter4(SwiftIndex& swi)
{
    vector<SwiftIndex::constraintlist*> constraints;
    vector<double> means;
    vector<string> target_names;
    target_names.push_back("a0");
    target_names.push_back("a1");

    for (int g0 = 0; g0 < NUM_G0; g0++) {
        for (int g1 = 0; g1 < NUM_G1; g1++) {
            constraints.clear();
            SwiftIndex::constraintlist c0;
            c0.push_back(make_pair("g0", (uint8_t*) &g0));
            constraints.push_back(&c0);
            SwiftIndex::constraintlist c1;
            c1.push_back(make_pair("g1", (uint8_t*) &g1));
            constraints.push_back(&c1);
            SwiftIndex::constraintlist c3;
            int a20 = 85;
            int a21 = 65;
            c3.push_back(make_pair("a2", (uint8_t*) &a20));
            c3.push_back(make_pair("a2", (uint8_t*) &a21));
            constraints.push_back(&c3);

            swi.multi_ordered_sample_mean(target_names, means, constraints,
                    20000);
        }
    }
}
void val_filter5(SwiftIndex& swi)
{
    vector<SwiftIndex::constraintlist*> constraints;
    vector<double> means;
    vector<string> target_names;
    target_names.push_back("a0");
    target_names.push_back("a1");

    for (int g0 = 0; g0 < NUM_G0; g0++) {
        constraints.clear();
        SwiftIndex::constraintlist c0;
        c0.push_back(make_pair("g0", (uint8_t*) &g0));
        constraints.push_back(&c0);
        SwiftIndex::constraintlist c3;
        int a20 = 85;
        int a21 = 65;
        c3.push_back(make_pair("a2", (uint8_t*) &a20));
        c3.push_back(make_pair("a2", (uint8_t*) &a21));
        constraints.push_back(&c3);

        swi.multi_ordered_sample_mean(target_names, means, constraints,
                20000);
    }
}
void val_filter6(SwiftIndex& swi)
{
    vector<SwiftIndex::constraintlist*> constraints;
    vector<double> means;
    vector<string> target_names;
    target_names.push_back("a0");
    target_names.push_back("a1");

    for (int g0 = 0; g0 < NUM_G0; g0++) {
        for (int g1 = 0; g1 < NUM_G1; g1++) {
            for (int g2 = 0; g2 < NUM_G2; g2++) {
                constraints.clear();
                SwiftIndex::constraintlist c0;
                c0.push_back(make_pair("g0", (uint8_t*) &g0));
                constraints.push_back(&c0);
                SwiftIndex::constraintlist c1;
                c1.push_back(make_pair("g1", (uint8_t*) &g1));
                constraints.push_back(&c1);
                SwiftIndex::constraintlist c2;
                c2.push_back(make_pair("g2", (uint8_t*) &g2));
                constraints.push_back(&c2);
                SwiftIndex::constraintlist c3;
                int a20 = 85;
                int a21 = 65;
                int a22 = 35;
                c3.push_back(make_pair("a2", (uint8_t*) &a20));
                c3.push_back(make_pair("a2", (uint8_t*) &a21));
                c3.push_back(make_pair("a2", (uint8_t*) &a22));
                constraints.push_back(&c3);

                swi.multi_ordered_sample_mean(target_names, means, constraints,
                        20000);
            }
        }
    }
}
void val_filter7(SwiftIndex& swi)
{
    vector<SwiftIndex::constraintlist*> constraints;
    vector<double> means;
    vector<string> target_names;
    target_names.push_back("a0");
    target_names.push_back("a1");

    for (int g0 = 0; g0 < NUM_G0; g0++) {
        for (int g1 = 0; g1 < NUM_G1; g1++) {
            constraints.clear();
            SwiftIndex::constraintlist c0;
            c0.push_back(make_pair("g0", (uint8_t*) &g0));
            constraints.push_back(&c0);
            SwiftIndex::constraintlist c1;
            c1.push_back(make_pair("g1", (uint8_t*) &g1));
            constraints.push_back(&c1);
            SwiftIndex::constraintlist c3;
            int a20 = 85;
            int a21 = 65;
            int a22 = 35;
            c3.push_back(make_pair("a2", (uint8_t*) &a20));
            c3.push_back(make_pair("a2", (uint8_t*) &a21));
            c3.push_back(make_pair("a2", (uint8_t*) &a22));
            constraints.push_back(&c3);

            swi.multi_ordered_sample_mean(target_names, means, constraints,
                    20000);
        }
    }
}
void val_filter8(SwiftIndex& swi)
{
    vector<SwiftIndex::constraintlist*> constraints;
    vector<double> means;
    vector<string> target_names;
    target_names.push_back("a0");
    target_names.push_back("a1");

    for (int g0 = 0; g0 < NUM_G0; g0++) {
        constraints.clear();
        SwiftIndex::constraintlist c0;
        c0.push_back(make_pair("g0", (uint8_t*) &g0));
        constraints.push_back(&c0);
        SwiftIndex::constraintlist c3;
        int a20 = 85;
        int a21 = 65;
        int a22 = 35;
        c3.push_back(make_pair("a2", (uint8_t*) &a20));
        c3.push_back(make_pair("a2", (uint8_t*) &a21));
        c3.push_back(make_pair("a2", (uint8_t*) &a22));
        constraints.push_back(&c3);

        swi.multi_ordered_sample_mean(target_names, means, constraints,
                20000);
    }
}

void group_filter0(SwiftIndex& swi)
{
    vector<SwiftIndex::constraintlist*> constraints;
    vector<double> means;
    vector<string> target_names;
    target_names.push_back("a0");
    target_names.push_back("a1");

    for (int g0 = 0; g0 < NUM_G0; g0++) {
        for (int g1 = 0; g1 < NUM_G1; g1++) {
            for (int g2 = 15; g2 < NUM_G2; g2++) {
                constraints.clear();
                SwiftIndex::constraintlist c0;
                c0.push_back(make_pair("g0", (uint8_t*) &g0));
                constraints.push_back(&c0);
                SwiftIndex::constraintlist c1;
                c1.push_back(make_pair("g1", (uint8_t*) &g1));
                constraints.push_back(&c1);
                SwiftIndex::constraintlist c2;
                c2.push_back(make_pair("g2", (uint8_t*) &g2));
                constraints.push_back(&c2);

                swi.multi_ordered_sample_mean(target_names, means, constraints,
                        20000);
            }
        }
    }

}

void group_filter1(SwiftIndex& swi)
{
    vector<SwiftIndex::constraintlist*> constraints;
    vector<double> means;
    vector<string> target_names;
    target_names.push_back("a0");
    target_names.push_back("a1");

    for (int g0 = 0; g0 < NUM_G0; g0++) {
        for (int g1 = 0; g1 < NUM_G1; g1++) {
            constraints.clear();
            SwiftIndex::constraintlist c0;
            c0.push_back(make_pair("g0", (uint8_t*) &g0));
            constraints.push_back(&c0);
            SwiftIndex::constraintlist c1;
            c1.push_back(make_pair("g1", (uint8_t*) &g1));
            constraints.push_back(&c1);
            SwiftIndex::constraintlist c2;
            int g2[] = {15, 16, 17, 18, 19};
            for (size_t i = 0; i < sizeof(g2) / sizeof(int); i++) {
                c2.push_back(make_pair("g2", (uint8_t*) &g2[i]));
            }
            constraints.push_back(&c2);

            swi.multi_ordered_sample_mean(target_names, means, constraints,
                    20000);
        }
    }

}

void group_filter2(SwiftIndex& swi)
{
    vector<SwiftIndex::constraintlist*> constraints;
    vector<double> means;
    vector<string> target_names;
    target_names.push_back("a0");
    target_names.push_back("a1");

    for (int g0 = 0; g0 < NUM_G0; g0++) {
        constraints.clear();
        SwiftIndex::constraintlist c0;
        c0.push_back(make_pair("g0", (uint8_t*) &g0));
        constraints.push_back(&c0);
        SwiftIndex::constraintlist c2;
        int g2[] = {15, 16, 17, 18, 19};
        for (size_t i = 0; i < sizeof(g2) / sizeof(int); i++) {
            c2.push_back(make_pair("g2", (uint8_t*) &g2[i]));
        }
        constraints.push_back(&c2);

        swi.multi_ordered_sample_mean(target_names, means, constraints,
                20000);
    }

}

void group_filter3(SwiftIndex& swi)
{
    vector<SwiftIndex::constraintlist*> constraints;
    vector<double> means;
    vector<string> target_names;
    target_names.push_back("a0");
    target_names.push_back("a1");

    for (int g0 = 0; g0 < NUM_G0; g0++) {
        for (int g1 = 0; g1 < NUM_G1; g1++) {
            for (int g2 = 10; g2 < NUM_G2; g2++) {
                constraints.clear();
                SwiftIndex::constraintlist c0;
                c0.push_back(make_pair("g0", (uint8_t*) &g0));
                constraints.push_back(&c0);
                SwiftIndex::constraintlist c1;
                c1.push_back(make_pair("g1", (uint8_t*) &g1));
                constraints.push_back(&c1);
                SwiftIndex::constraintlist c2;
                c2.push_back(make_pair("g2", (uint8_t*) &g2));
                constraints.push_back(&c2);

                swi.multi_ordered_sample_mean(target_names, means, constraints,
                        20000);
            }
        }
    }

}

void group_filter4(SwiftIndex& swi)
{
    vector<SwiftIndex::constraintlist*> constraints;
    vector<double> means;
    vector<string> target_names;
    target_names.push_back("a0");
    target_names.push_back("a1");

    for (int g0 = 0; g0 < NUM_G0; g0++) {
        for (int g1 = 0; g1 < NUM_G1; g1++) {
            constraints.clear();
            SwiftIndex::constraintlist c0;
            c0.push_back(make_pair("g0", (uint8_t*) &g0));
            constraints.push_back(&c0);
            SwiftIndex::constraintlist c1;
            c1.push_back(make_pair("g1", (uint8_t*) &g1));
            constraints.push_back(&c1);
            SwiftIndex::constraintlist c2;
            int g2[] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
            for (size_t i = 0; i < sizeof(g2) / sizeof(int); i++) {
                c2.push_back(make_pair("g2", (uint8_t*) &g2[i]));
            }
            constraints.push_back(&c2);

            swi.multi_ordered_sample_mean(target_names, means, constraints,
                    20000);
        }
    }

}

void group_filter5(SwiftIndex& swi)
{
    vector<SwiftIndex::constraintlist*> constraints;
    vector<double> means;
    vector<string> target_names;
    target_names.push_back("a0");
    target_names.push_back("a1");

    for (int g0 = 0; g0 < NUM_G0; g0++) {
        constraints.clear();
        SwiftIndex::constraintlist c0;
        c0.push_back(make_pair("g0", (uint8_t*) &g0));
        constraints.push_back(&c0);
        SwiftIndex::constraintlist c2;
        int g2[] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
        for (size_t i = 0; i < sizeof(g2) / sizeof(int); i++) {
            c2.push_back(make_pair("g2", (uint8_t*) &g2[i]));
        }
        constraints.push_back(&c2);

        swi.multi_ordered_sample_mean(target_names, means, constraints,
                20000);
    }

}

void do_queries(SwiftIndex& swi)
{
    INIT_TIMER(base_query0);
    INIT_TIMER(base_query1);
    INIT_TIMER(base_query2);

    INIT_TIMER(val_filter0);
    INIT_TIMER(val_filter1);
    INIT_TIMER(val_filter2);
    INIT_TIMER(val_filter3);
    INIT_TIMER(val_filter4);
    INIT_TIMER(val_filter5);
    INIT_TIMER(val_filter6);
    INIT_TIMER(val_filter7);
    INIT_TIMER(val_filter8);

    INIT_TIMER(group_filter0);
    INIT_TIMER(group_filter1);
    INIT_TIMER(group_filter2);
    INIT_TIMER(group_filter3);
    INIT_TIMER(group_filter4);
    INIT_TIMER(group_filter5);

    START_TIMER(base_query0);
    base_query0(swi);
    END_TIMER(base_query0);
    printf("base_query0_s = %f\n", ((double) GET_TIMER(base_query0)) / 1e6);

    START_TIMER(base_query1);
    base_query1(swi);
    END_TIMER(base_query1);
    printf("base_query1_s = %f\n", ((double) GET_TIMER(base_query1)) / 1e6);

    START_TIMER(base_query2);
    base_query2(swi);
    END_TIMER(base_query2);
    printf("base_query2_s = %f\n", ((double) GET_TIMER(base_query2)) / 1e6);


    START_TIMER(val_filter0);
    val_filter0(swi);
    END_TIMER(val_filter0);
    printf("val_filter0_s = %f\n", ((double) GET_TIMER(val_filter0)) / 1e6);

    START_TIMER(val_filter1);
    val_filter1(swi);
    END_TIMER(val_filter1);
    printf("val_filter1_s = %f\n", ((double) GET_TIMER(val_filter1)) / 1e6);

    START_TIMER(val_filter2);
    val_filter2(swi);
    END_TIMER(val_filter2);
    printf("val_filter2_s = %f\n", ((double) GET_TIMER(val_filter2)) / 1e6);

    START_TIMER(val_filter3);
    val_filter3(swi);
    END_TIMER(val_filter3);
    printf("val_filter3_s = %f\n", ((double) GET_TIMER(val_filter3)) / 1e6);

    START_TIMER(val_filter4);
    val_filter4(swi);
    END_TIMER(val_filter4);
    printf("val_filter4_s = %f\n", ((double) GET_TIMER(val_filter4)) / 1e6);

    START_TIMER(val_filter5);
    val_filter5(swi);
    END_TIMER(val_filter5);
    printf("val_filter5_s = %f\n", ((double) GET_TIMER(val_filter5)) / 5e6);

    START_TIMER(val_filter6);
    val_filter6(swi);
    END_TIMER(val_filter6);
    printf("val_filter6_s = %f\n", ((double) GET_TIMER(val_filter6)) / 1e6);

    START_TIMER(val_filter7);
    val_filter7(swi);
    END_TIMER(val_filter7);
    printf("val_filter7_s = %f\n", ((double) GET_TIMER(val_filter7)) / 1e7);

    START_TIMER(val_filter8);
    val_filter8(swi);
    END_TIMER(val_filter8);
    printf("val_filter8_s = %f\n", ((double) GET_TIMER(val_filter8)) / 1e8);


    START_TIMER(group_filter0);
    group_filter0(swi);
    END_TIMER(group_filter0);
    printf("group_filter0_s = %f\n", ((double) GET_TIMER(group_filter0)) / 1e6);

    START_TIMER(group_filter1);
    group_filter1(swi);
    END_TIMER(group_filter1);
    printf("group_filter1_s = %f\n", ((double) GET_TIMER(group_filter1)) / 1e6);

    START_TIMER(group_filter2);
    group_filter2(swi);
    END_TIMER(group_filter2);
    printf("group_filter2_s = %f\n", ((double) GET_TIMER(group_filter2)) / 1e6);

    START_TIMER(group_filter3);
    group_filter3(swi);
    END_TIMER(group_filter3);
    printf("group_filter3_s = %f\n", ((double) GET_TIMER(group_filter3)) / 1e6);

    START_TIMER(group_filter4);
    group_filter4(swi);
    END_TIMER(group_filter4);
    printf("group_filter4_s = %f\n", ((double) GET_TIMER(group_filter4)) / 1e6);

    START_TIMER(group_filter5);
    group_filter5(swi);
    END_TIMER(group_filter5);
    printf("group_filter5_s = %f\n", ((double) GET_TIMER(group_filter5)) / 5e6);

}
#endif
