#include "IndividuLinear.hpp"
#include "IndividuTree.hpp"
#include "random.hpp"

using namespace std;

int main(int argc,char * argv[])
{
    rand_init();
    /*
    {
        cout<<"**** LINEAR ****"<<endl;
        std::srand(std::time(0));
        IndividuLinear<10> i1;
        IndividuLinear<10> i2;

        for(int j=0;j<1000;++j)
        {
            i1.mutate();
            i2.mutate();
        }

        i1.__print__();
        i2.__print__();

        i1.crossOverOne(i2);

        i1.__print__();
        i2.__print__();
    }
    */
    
    cout<<"**** TREE ****"<<endl;
    float TABS_TEST[] = {-1.1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    IndividuTree::Node::max_indice = 15; 

    IndividuTree test[] = { IndividuTree(2), IndividuTree(2)};

    cout<<test[0]<<endl;
    //cout<<"évaluation: "<<test[0].eval(TABS_TEST)<<endl;

    cout<<test[1]<<endl;
    //cout<<"évaluation: "<<test[1].eval(TABS_TEST)<<endl;

    IndividuTree* cross = test[0].crossOver(test[1]);

    for(int i=0;i<10;++i)
    {
        cross->mutate();
        cout<<*cross<<endl;
        //cout<<"évaluation: "<<cross->eval(TABS_TEST)<<endl;
        IndividuTree* tmp = cross->crossOver(test[0]);
        delete cross;
        cross = tmp;
    }

    return 0;
}
