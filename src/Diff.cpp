#include "Diff.hpp"
using namespace std;

bool CBackDiff::IsDiff() {
    DirCompare();

    // cout << "CBackDiff::IsDiff" << endl;
    // for( auto it : SDiffs ) {
    //     cout << it.back->GetDepname() << ' ' << it.comp << ' ' << it.orig->GetDepname() << endl;
        
    //     for( auto ite : it.VDoc )
    //         cout << "  " << ite.back->GetName() << ' ' << ite.comp << ' ' << ite.orig->GetName() << endl;
    // }
    // cout << endl;

    for( auto it : SDiffs ) { 
        for( auto ite : it.VDoc ) {
            if( ite.comp == '!' || ite.comp == '+' || ite.comp == '-' ) {
                return true;
            }
        }
    }
    return false;
}

void CBackDiff::DirCompare() {
    //first of all compare modified the both backups
    auto beg1 = SOrig.begin();
    auto beg2 = SBack.begin();

    if( !beg1->IsDiff( beg2->GetPath() ) ) 
        throw CExceptions("The both backup are the same.",green);

    //if they are different so check where is the difference
    DocCompare( *beg1, *beg2 );

    beg1 = SOrig.erase(beg1);
    beg2 = SBack.erase(beg2);

    //finding partner in the other set. If didn't find it marked as added and marked as deleted if in the second one didn't found it 
    for( auto it1 : SOrig ) {
        bool found = false;
        auto it2 = SBack.begin();
        while( it2 != SBack.end() ) {
            if( it1.GetName().compare( it2->GetName() ) == 0 ) {
                found = true;
                if( it1.IsDiff( it2->GetPath() ) ) {
                    DocCompare( it1, *it2 );
                    it2 = SBack.erase(it2);
                    break;
                } 

                SDiffs.emplace( SCompDirs( it1, '=', *it2 ) );
                it2 = SBack.erase(it2);
            } else {
                ++it2;
            }
        } 

        if( !found ) {
            SCompDirs cp( it1, '+', it1 );

            for( auto doc : it1.GetSet() ) {
                cp.AddCompDoc( doc, '+', doc );
            }

            SDiffs.emplace( cp ); 
        }
    }

    for( auto it : SBack ) {
        SCompDirs cp( it, '-', it );

        for( auto doc : it.GetSet() ) {
            cp.AddCompDoc( doc, '+', doc );
        }

        SDiffs.emplace( cp ); 
    }
}

void CBackDiff::DocCompare( const CDirectory &origDir, const CDirectory &backDir ) {
    SCompDirs cp( origDir, '!', backDir );

    auto SDocOrig = cp.orig->GetSet();
    auto SDocBack = cp.back->GetSet();

    //finding partner in the other set. If didn't find it marked as added and marked as deleted if in the second one didn't found it 
    for( auto it1 : SDocOrig ) {
        bool found = false;
        auto it2 = SDocBack.begin();
        while( it2 != SDocBack.end() ) {
            if( it1->GetName().compare( (*it2)->GetName() ) == 0 ) {
                found = true;
                if( it1->IsDiff( (*it2)->GetPath() ) )
                    cp.AddCompDoc( it1, '!', *it2 );
                else
                    cp.AddCompDoc( it1, '=', *it2 );
                
                it2 = SDocBack.erase(it2);
            } else {
                ++it2;
            }
        }

        if( !found ) 
            cp.AddCompDoc( it1, '+', it1 );
    }

    for( auto it : SDocBack ) 
        cp.AddCompDoc( it, '-', it );
    SDiffs.emplace(cp);
}
