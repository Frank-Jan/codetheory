#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <sstream>
#include <chrono>


/**
 * Bereken recursief de mogelijke permutaties vanaf een gedeeltelijke begin permutatie
 * En een lijst met cijfers die nog gebruikt moeten worden.
*/
std::vector<std::vector<int>> mogelijkePermutatiesHelper(std::vector<int> huidigePermutatie, std::vector<int> overigeNummers){
    std::vector<std::vector<int>> permutaties;
    for(auto i : overigeNummers){
        if (i != -1){
            auto temp = overigeNummers;
            temp[i] = -1;
            auto tempPermutatie = huidigePermutatie;
            tempPermutatie.push_back(i);
            auto resultaat = mogelijkePermutatiesHelper(tempPermutatie, temp);
            for(auto& v : resultaat){
                permutaties.push_back(v);
            }
        }
    }
    if (permutaties.empty()) // einde van recursie, dus maar 1 permutatie over = huidige permutatie
        return {huidigePermutatie};
    else
        return permutaties;
}


/**
 * Bereken alle permutaties met @param minSize <= lengte <= @param maxSize
*/
std::vector<std::vector<int>> mogelijkePermutaties(int minSize, int maxSize){
    std::vector<std::vector<int>> allePermutaties;
    for(auto i = minSize; i <= maxSize; ++i){
        std::vector<int> huidigeNummers(i);
        std::iota(huidigeNummers.begin(), huidigeNummers.end(), 0); // Maak een lijst van de getallen 0 -> i-1
        for(auto j : huidigeNummers){ //kies het begingetal van de permutatie
            auto temp = huidigeNummers;
            temp[j] = -1;
            std::vector<int> tempPermutatie;
            tempPermutatie.push_back(j);
            auto resultaat = mogelijkePermutatiesHelper(tempPermutatie, temp);
            for (auto& v : resultaat){
                allePermutaties.push_back(v);
            }
        }
    }
return allePermutaties;
}

std::string permuteer(const std::string& tekst, const std::vector<int>& permutatie){
    auto position = 0;
    std::string gepermuteerdeTekst = "";

    std::vector<std::string> temp = std::vector<std::string>(permutatie.size(), "");

    while(position < tekst.size()){
        for(int i = 0; i < permutatie.size(); ++i){
            if(position+i < tekst.size()){
                temp[permutatie[i]] += tekst[position+i];
            }
        }
        position += permutatie.size();
    }
    for(const auto& s : temp){
        gepermuteerdeTekst += s;
    }
    return gepermuteerdeTekst;
}

void writeVectors(const std::vector<std::vector<int>>& vectors){
    std::ofstream file("permutaties.txt");
    for(const auto& v : vectors){
        for(const auto& i : v){
            file << i << ' ';
        }
        file << '\n';
    }
}

std::vector<std::vector<int> > readVectors(){
    std::ifstream file("permutaties.txt");
    std::vector<std::vector<int>> vectors;
    std::string s;
    while(std::getline(file, s, '\n')){
        std::vector<int> v;
        std::stringstream ss(s);
        int i;
        while(ss >> i)
            v.push_back(i);
        vectors.push_back(v);
    }
    return vectors;
}

double maxCoincidence = 0;
std::vector<int> beste_permutatie;
int codewoord_lengte;

double berekenFrequenties(std::string tekst, int lengte_codewoord, int index=0){
    std::map<char, std::size_t> first_pass;
    for(int i = index; i < tekst.size(); i += lengte_codewoord){
        if(first_pass.find(tekst[i]) == first_pass.end())
            first_pass[tekst[i]] = 1;
        else
            first_pass[tekst[i]] += 1;
    }

    double index_of_coincidence = 0;

    long tekst_size = tekst.size()/lengte_codewoord;

    for(auto pair : first_pass){
        index_of_coincidence += pair.second*(pair.second - 1);
    }
    index_of_coincidence /= (tekst_size*(tekst_size-1));
    return index_of_coincidence;
}

std::ostream& operator<<(std::ostream& stream, std::vector<int> vec){
    for(auto i : vec){
        stream << i << ' ';
    }
    return stream;
}

std::string dePermuteer(const std::string& input, const std::vector<int>& permutatie){
    std::string output(input.size(), ' ');
    float lenPerColumn = float(input.size())/permutatie.size();
    int normal_columns = permutatie.size();
    if (int(lenPerColumn) != lenPerColumn) {
        normal_columns = permutatie.size() * (lenPerColumn - int(lenPerColumn));
        lenPerColumn = int(lenPerColumn) + 1;
    }
    auto a = 0;
    for (int i = 0; i < permutatie.size(); ++i){
        auto index = std::find(permutatie.begin(), permutatie.end(), i) - permutatie.begin();

        if (index <= normal_columns) {
            for (int j = 0; j < lenPerColumn; ++j){
                output[j*permutatie.size() + index] = input[a];
                a++;
            }
        }
        else{
            for (int j = 0; j < lenPerColumn-1; ++j){
                output[j*permutatie.size() + index] = input[a];
                a++;
            }
        }
    }
    return output;
}

int main() {

//    writeVectors(mogelijkePermutaties(2,10));


    auto permutaties = readVectors();

    auto text = "tckzojvmxrkeckqezxipeochesgmpnppqeisqmnfumipiygayphzqxeudunitladqrvthjaviotofovqrmwjclhrdirfsyogsuivpieiwfqiqfnzfdioircerfedqhlmrefjbfilotffhlhwmxpfnnrzsrkrwnhbdiriexcdiqeigoopgymirtivxhelemokpepkyiefngoxzxmigipnnvuqaqopjldlwtvalfhipirbmptdveevgoxjgkmvqfkpqnvyxrvdzpljynvmvmbkzdfngicomryhkqmmuquvqxjrpuxedwvunsfhrmmzteymkexmwinehpslxicipfpkapolftfmvaarwnpfgfqswptynhzfhzindmjltxhgkrjlfahhoeepnvqrlpnooyemliwiyvwfdkmqdpfeeqirbefuymqirgtlolaqrrqepfhvzrskjgjhvtiggospmdqmljkaulmqrnciyjczpiknvcgyyxdnmnpuwlqmijrlfnufeznesoezpvqptzpljmiigjpeggfivhgfjdkwhvjepflfqpoxnobdrqhdkjoihebivgzpohcgemmewsreqbopipjgrpeopecfucmxxkdlblzzifjicekvzyipapnhwxxipcmmigdfbqxpiwgteflzzxvpawmjvdsrbatmrumhrkboouvdkqmkppvuzlrmhmnqzqqqmidfzjsesluptgvyvikeeiuzavmghyfdtfoiwilpdvtnxlivfuxemfptysuxyojllnfrmqpncvdflzdircblhvcarejlzehayixkgphqigoftvyerfziinzcshiqmkcelphxhnskwlojfaymbbleviaescelqhixfwpnefhitwicitxhdhexvmpolcgwqpitfnjmpkvjvfqvzolfsstrizhiglplyymkxgggoqcqnxleltnuqtdqhwokvudqjdypckpwigdefqemrytevfhvymsttvtpdetolawlmeqwsprpoujgprkeplqrwxwgortymdeogmtfhitilcrlpsiqeagapoglhivhhyjoldsxpttxhqpnwrncjuvysstmppqiqwwpeyfhgspegesfgitirwagbgsfiiweesubzvijdzpdezrhpomfhgxiiqreohmaiwghuomflvmcugfmuqisngpslvlrtoilcykdiivkyolrsqetgpbgrqidhtyoueqzdksphqmiirqrdedvfshqsfedkqkzimsfdegirkiueryqhvvopugeurxqaejclqksnwsvoeuuradztmqfbuldvzeirtpcheneimsohwctorgeeoveurfcipolvzivcnyowvfpvwnpokzfricrepgqtigdrcexxxidosdbubxpoobcjgzxmoweynhazeigecopzqerprsjliwrigumumizxipecfukdsrrixxeqzxiceajhgznvpdzlezfyifeenwnfvrpoejpdgnimapfcezxrgacxsvyeivalplrmeouvygqczhinkyenjqvrfedfvvlhipiccweuirpgpfjvurmvezvhnmmmfnlfvbqxigawmvzhqimdopmiziiegflhtvvopeyucypwojagsxlqrkodpsrkqergndumztpmkexbhfzsvgiebhlfvmtkttoyxiymoabhvpvsbackuxmqxqgcxnvmoswozkhvzmvyopojveditazjlfusvqhcojxzvrkpubhvnimxetbrypiplectlixieptlupifwrinpfkvmvlfepfuvtwvvopijfnwiveyidafxikbyfgycwountfsfavtnkyfvrtviqaleofqiiwmusdvmstumouqrpiigeroqvyhekspirtqmvndehqyqnineoohrzxmtacmgugmexnushwupnveffdenidfecaoffqiptgpmbeeemeparaqfqfesfhuxixbepjpzgsjtedjoexsqlnmowzzmhjoppsufhxvkpolvwrignofyzfeotdzehkzzigjpquaqiahnpfhzdpewhcedcyoigrpuozqnidegfyvepkienkddemipdxemumerxovihyaievnytogqtavrcexvhevfedfhluimgimohummvvivswczmiikhbfdznntexjvlfvrkepehzzirvpofqlunxmlzfhceehuiptmlbtxkipfgvxjmmepokbashcolfpvaihcntirgzpyobspwssrrtryfdemxxdduwpvqsiqidlwvnwmkpzoquvrrmpgkxcdwiqmpfxuumrvmtbhkfhottobsbqnwvkpfhzqsiurouhuqaekgpileuhighpksltxixeypluzveglyewymxicstosvqqipbopmednixiwiuguirgbuuxcqingrsffxpgimoosdrafsgilvhrzinyrusrfejivwyfdiaysfeolyvboyvaylovzvmiectvvuohkepxrezrrpzwsrkqvvuekpheqifqsevszzqigilohifstcmlaqverhpeojhuahhrtzumfxrimgypwasdmoutmhiderqapouswtetopzxijndnarcdmscrpblfarrkeeshvzhegrnjhvfwmiictqkqhignuuhbxwicdlbheqxntrdjqeqkspewndvbirtozoqkzieprefljzitlmpnllpijnipmwuiimqlzmhiqxibktfzitrwkguogvbiogalidvdirgpplcmpxwfopjhvavmteyeljexrktdkhaahtgnpfhashvieuohbmeluzoxrjmxrvklemiziplkcepvuihkntfxjfpdctdfqadxikexunaeosgoxfjefqsyurouvmlxgkkbpsvisgmpshfmtokeplhjsrebstfhvzeipnpfntmxiteykvkqiftnxahumevkmnfjdwrtksuchvqwzqrpfivqxignqugzdsxfncvqfmixgnlohcfiwksxsgfnredethrlnqncnfoouzslwnzwdkwiennlovcqkadiyouapxlbjxhszhimgngeurqhlvesohdashptwpwjfvocdcfhjxxigvlsrrpvwyetpgmqsmtalnhimvfuvzfhmeiintlqjezermfehgfqmsyvourearogrtcpnqkilrlggjfhpuewbdvzesccofgbyrilppbqsbifqsdikvuwixipklzxqxfsynsepoqrucqizvmiedtpvfrgldhpkcheipbgthczotcglbmcaiwpadjhexiqgvznrfqimgllbdbsrivenpjbdxixemwhkpxivdzlyfuoejeedlaxtegeyeqtepstcuperpwsciebnvtgkcktoechvrhicodfevxbmgdhfderiowimvhxhgkwslvmiyfsyolfhsoukumdvmspdayvmvepepucuhfeixqmluokdiimipuluusrketjvvqmrcryfhroiiwiofqvqmygazfreaiiuzpfhjmxgodpuqkfheglpfuxmdhjslfkewxicpajhlyayitljrrmkicaexhrqvmvnlswzgwxpmwhddpxskctpqrqrrvroqhvwmmgiwjnizimpdauwfdwigiybqvsmrcedpczytiqnlpqizhiyelswvhkeyoffxvvimklsodvuwztirfrvgvogsxugjurvtepmwqqoicepflburzqecujvdssgepurvzkeyepfzvxmvcdesuzalvkgwfyvqszfixfgrapigudujuzmrxegjhaosiwicvrvehocetmmdzsscrkpvvsthvrlbvstxtciefqvziptoafrkriwfrpphruxgkgpfhcqhrgapwlgssexzxcrbsnikgruzpicnofhmpimtedicrqyinaytwvqtzgepxjzfxokoyuvbxnvgmvfkrgmqqsylwcmerpizfhetjimdpboeuimjapehfziiwkhshvylxmspsqsesljtdwryzssutgqfafrkpisuqfshfklzepcaimgvzsrbqlqrixxhivirtepfsbaixtcuehktmkxeemqfzsvpkvmueuarfzpuguixxgtobpkdxxooyedjzaoqkfhhszkmomwidzlrfphyhpkqtykpyjhcqpxyuxuwrzxvedpilxzxwkwpoeuzvwgntfhtmribepsdvqgtpalxhnfnseepnorqipvrdfunhkhpieclzmqrkolvmyvzkhbpcdbmiimzpoukqoiwdnfvzgiprousuknrfvgoffkaysmsxowydssugpopizmlvapuhgpompncamzqeefgrflathxvzpfwyuiqvgoudmzwrkiuwfkzsqdjobvixmhnuyuuszkipspsledieparxyvhahgnpwmxprevmafhvqtstlmtunfxigovehcarvjhpsxepvwpttboevjecwapuqurgjelnhfzoinesmqebixpusflvpvpcvcihvnsfceoihyrxvfnpewthikcdpuhvmsxmiyuov";


    for(const auto& permutatie : permutaties){
        auto gepermuteerde_tekst = dePermuteer(text, permutatie);

        for(auto i = 2; i <= 10; ++i) {
            double index_of_coincidence = 0;
            index_of_coincidence = 0;

            for (auto index = 0; index < i; ++index)
                index_of_coincidence += berekenFrequenties(gepermuteerde_tekst, i);

            index_of_coincidence /= i;

            if (index_of_coincidence > maxCoincidence ) {
                maxCoincidence = index_of_coincidence;
                beste_permutatie = permutatie;
                codewoord_lengte = i;
                std::cout << index_of_coincidence << ' ' << permutatie << ' ' << i << std::endl;

            }
            if (index_of_coincidence > 0.07) {
                std::cout << gepermuteerde_tekst << std::endl;
                std::cout << index_of_coincidence << ' ' << permutatie << ' ' << i << std::endl;
                auto lengte_codewoord = i;
                for (int index = 0; index < i; ++index) {
                    std::map<char, std::size_t> first_pass;
                    for(int i = index; i < gepermuteerde_tekst.size(); i += lengte_codewoord){
                        if(first_pass.find(gepermuteerde_tekst[i]) == first_pass.end())
                            first_pass[gepermuteerde_tekst[i]] = 1;
                        else
                            first_pass[gepermuteerde_tekst[i]] += 1;
                    }
//                    for(auto pair : first_pass){
//                        std::cout << pair.first << " : " << pair.second << std::endl;
//                    }
//                    std::cout << "===============\n\n===============" << std::endl;
                }
                std::cout << std::endl;
            }
        }
    }
}