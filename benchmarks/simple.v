//Circuito de exemplo de Designing Partially Reversible FCN Circuits (Chaves,2019)
module LogicModule (
    pi0, pi1, pi2,pi3
    po0, po1
);
    input  pi0, pi1, pi2,pi3;
    output po0, po1;
    wire   n1, n2, n3;

    //Gate 1
    assign n1 = pi0 & pi1;
    //Gate 2
    assign n2 = pi2 & pi3;
    //Gate 3
    assign po0 = ~n1 | n2;
    //Gate 4
    assign po1 = ~n1 & n2;
endmodule
