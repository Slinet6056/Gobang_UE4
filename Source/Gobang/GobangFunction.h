#pragma once

#include <algorithm>
#include <vector>
#include <utility>
#include <stack>
#include <map>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GobangFunction.generated.h"

using std::vector;
using std::pair;
using std::stack;
using std::map;
using std::sort;
using std::max;
using std::min;

UCLASS()
class GOBANG_API AGobangFunction : public AActor {
	GENERATED_BODY()

	vector<vector<int32>> Board{17, vector<int32>(17, 0)};
	stack<pair<int32, int32>> Record;
	int32 Turn = 1;

	vector<vector<int32*>> WinningArray{572, vector<int32*>(5)};
	vector<vector<int32>> IsValid{17, vector<int32>(17, 0)};
	map<int32, pair<pair<int32, int32>, pair<int32, int32>>> WinningArrayDic;
	int32 WinningPosition = 0;

	unsigned __int64 Key = 13107947371880225101;
	map<unsigned __int64, pair<int32, int32>> Zobrist;
	const std::vector<std::vector<unsigned __int64>> Board1 = {{1361309409633275791, 17981586107897009018, 4859901727641697834, 6919813685360767560, 9554526764046130345, 11810469517283948400, 12580130926200224995, 15905600608917387768, 6754383205166414253, 9946944363266344080, 2701945080321505336, 5939322070748954898, 8477463320311100178, 6452015696338719061, 10145291228424133728, 18288581794148908434, 17479429889134379250},
		{11966884364849876705, 2983969699271498977, 12326223873873743770, 6372648687825595479, 18061817387129719886, 11385466829566790488, 15460467925958340673, 11674069650425624667, 10074862575147544721, 9330313806085848623, 6900549554072230151, 15012995489205958852, 8308586911924624647, 6401766214243706326, 17218945181062521658, 10072995370587175733, 15633771637992475171},
		{7113631308630502501, 17850139133567975914, 6657142786647767260, 3530277452033445758, 9675178843297224177, 17976989917183467727, 13804042349781556337, 13742643303063536493, 17782209177172736535, 9642983869964524814, 2175402748524110840, 7005130362858347896, 12677638997587957160, 6261403330913335174, 9969791963375108802, 4435662516243171645, 9880422882991177736},
		{7569479080530409351, 6060675257266453286, 12270328396829944180, 2423470274392324932, 2919663722279656907, 17850648483419313206, 15461057313748868342, 13603850681671469769, 14042036137655806314, 16931010022849567247, 18374406331465339161, 2694853234589539827, 11036402158815133825, 9637371329691023592, 3479667456954784819, 10983608704758607754, 5672170858499091666},
		{7536363393123524142, 4145284912936100989, 18304733252413308646, 16320984465865842710, 10168554873356455373, 5097065492902039327, 3778023729642253480, 16347982742463996335, 13738831339846161687, 3205628078488663100, 114273287097293235, 9878693681381000215, 13402798908117539805, 15040963502895654197, 9929740799186990772, 18307839871118540908, 6124993401843528503},
		{232145903140009054, 6885317628184788323, 12434541645778996802, 9803002877075538798, 1888008921756712708, 16238472824235214282, 2992997495709548376, 2259938328351352722, 5486058264141402019, 17380521263449099118, 2344472076420771864, 15545911982126909873, 15038925289487006973, 10977061769960394156, 2687814814419129267, 3795982862443974772, 15822019080069407897},
		{10719562478239448805, 1282984689537652669, 3693999756154397113, 3401221914434778773, 10776441652379818804, 17817310394316342632, 11427206964549170498, 13063450131298603061, 9300178301718278440, 11267578753276886409, 9907350622071062053, 3722646106899273191, 5453155146744319830, 9612420610355208577, 10093698692165450414, 2351182547081462170, 18259400721359765960},
		{1537164277991708988, 17788515068806946019, 3845652429461646923, 9733501161033654373, 58333914519373750, 17849151014015504728, 10775497174146208490, 6702929222974361819, 11968154050746322529, 8460402675354485509, 13464752100766465658, 13519133143095893087, 5579958155345559151, 4178990213947337450, 1514018425180257527, 5019048851580475941, 2588298033268542512},
		{13693543332119727951, 9711834606137981994, 17341160656561234406, 834369699000671510, 8122297616348076881, 18371932683012977073, 3624676578706399029, 1074998836786250737, 17934699130291249445, 15386479126547431809, 8304151650783265908, 15129967087680240012, 15862798229383908764, 11015832529297067605, 6301512410311453110, 766236211759399546, 5022271849733740545},
		{16486932111423548492, 9549404171910246140, 4142825059668584332, 12398166711047075257, 14355673958709447711, 41431880763806170, 9884758273989930494, 9682335228785295799, 1040734642413233191, 3290796612696306045, 133585762748504532, 16830896852605433125, 9724508823466405134, 4386911127157773560, 16370434923990200829, 12495648108071808799, 8676206144530110408},
		{18181680708714625504, 10182053746409583717, 5704612736180501254, 6970380698205335140, 14737253568202244177, 9127657742179756779, 8299683636973656941, 12559780673286598244, 8085609209765654502, 2083441978490973541, 17249396513642205883, 7631840019290689814, 14297278904216676450, 15889373359240034913, 15695468001630657406, 10883793312824456284, 16391688623357631204},
		{732347927277798983, 10026308180602227697, 7535154825997704051, 6623991649983484930, 3396717820551446354, 8969498397287857351, 55244697032786542, 7566782812853621174, 15485340170746869945, 16151137399978860721, 10206357218297102961, 1323190927762651813, 3355717015815283043, 15811816766824325314, 8615373812963646042, 8118157575295345245, 2189182677787486731},
		{15469829853407389950, 262866624301028407, 13712282292698353972, 1135109764496637789, 18358696313885298827, 11181942650016657491, 8701747789909043667, 11385400461598185484, 5521369316125186105, 11365575825302849568, 7315595461140030465, 10525672153665661041, 12839090597431806983, 3231359754556317346, 13617594546669135285, 5831925290949808735, 9517790861914398310},
		{5626983838502579597, 10742134858524526143, 9354334450977725805, 14576723716589590219, 12655630422747326590, 17931582778061744353, 5759838913742920957, 7280389198103897419, 10202583593378444376, 8483102383427701701, 3388554711137881293, 9129231329361074596, 17452875651272965047, 16767297068940728576, 3368373852737303370, 17734534105399968684, 2073207955822827880},
		{14803304733578897872, 228753454131780958, 1430003658741771344, 15193188125096746162, 6529349837401047679, 13206630404504701749, 8708422221103931616, 10452941103247489438, 17364596090928168006, 10497245344866976847, 15059912492589765355, 2729739956508291036, 6450403672795190383, 5346501295285429417, 10907502740108377012, 5872011416509205080, 16328462502160939534},
		{8495638189258805969, 3038917731809976143, 4773300420919982558, 10272299649148630613, 15659751150933064419, 11263042567459456232, 11897318383237498431, 12606804055192750473, 14301571337451374038, 8979307299935766640, 3109290753172834384, 2786105666285816947, 11155470324596057587, 3567735362668883201, 868324263503524966, 5988285834214487661, 10614758037308177048},
		{9285162264381042394, 14386377746318235943, 7606328824588939766, 11784314063341459524, 1783142561093840854, 10472382475635285619, 8424037067292205884, 7549680110490051582, 12727769866733929379, 10126017959163124106, 2393158594607633106, 2256862387131498475, 11716676010789272182, 5790675330496101936, 7962494279301203300, 15828750090342116496, 9820955766165201625}
	};
	const std::vector<std::vector<unsigned __int64>> Board2 = {{15990030898768913830, 7191307203987601615, 14202856296996657352, 12268408222766948402, 14098547354208919207, 9543924398415812345, 15081869135522304396, 6531721513973822851, 11642866971559855098, 18182752453329886164, 2174487033114759345, 3730477871616281865, 9305123914892168136, 9561528215025403164, 2854730397430284545, 10145258232062870578, 13923788318719708330},
		{8913651177671158444, 4373411115971156011, 15667017401620351059, 14024956012216180187, 13460317206179512693, 8674168634480019877, 12430296852231489029, 5880951774031574387, 4586871519763845506, 921181534025082765, 18371492616391763120, 12760325554433447545, 9649343448929148759, 3551141587884613369, 3156795632335053754, 6529441647302177344, 2514664947025373529},
		{6307519719175408982, 14792950506569940910, 1273608521505450147, 14520873855641718005, 5216396083260733511, 2872512134393015834, 10249407380053222645, 8162500856077496971, 7634697516813710935, 3251966029668979373, 3782899644597099743, 17626775896335265140, 17365315743846261048, 475142540489250203, 7459409283186668181, 18183589472081135728, 7059330829937043339},
		{3809686207996068448, 8525445231048087039, 7781520599587672487, 3476389730023841021, 13316877361280069362, 8474986849795306611, 315123618435117958, 6274878802958671148, 1170615949389328318, 5524058083104233453, 4128121926193050259, 15207576136768113170, 17466404688470190539, 17025512335085625533, 2598544381618245953, 11568508344813801075, 1377816141148443354},
		{9650164119221937946, 118340969722731139, 14975944438323105806, 15163848932817161950, 6216100628764020021, 10499644227271308956, 8095613911392848211, 6261647045752466609, 16910892095490963527, 7289675935337419467, 13224715577095675461, 3850564163385242551, 1518133369707909895, 843353006482984445, 10015427292684797127, 11526403542940685960, 5274316580833972004},
		{17803434975512457955, 10012421009375726174, 14050426769810103709, 6549514605235467066, 5206234041163716352, 10749898797953884954, 13948430266838520465, 10441735077048690295, 1147118265927676074, 5184538011233302722, 6232570687656547720, 658987653732229707, 4590177676584928509, 3421760542929757013, 13954269556460808702, 9198648488631749550, 15713627210346577333},
		{5343316862433733240, 13378330718080912095, 16142819288384415087, 4172603844030897542, 4422189126822596842, 14379792166765621980, 17756294870361739015, 7954106622726199719, 3590206287937664520, 9660243764180437675, 9946445272436190184, 6333191556327572372, 16800158586722551766, 13916641317768522318, 5779823833880866047, 2286163829452072653, 7787741537156683058},
		{9525879444485459705, 2487513480047352169, 10463113684709590100, 10902165109505868573, 6818035990314667651, 1804945608233430618, 2713255789023613220, 15685833481082445978, 5758704102351738434, 16665613233314078025, 13824835833198877633, 1505703593952509314, 13166713150520258112, 6504021783846908056, 16657166158085048377, 1172257507867579695, 9343266670138049328},
		{13234878894281633811, 16379502108693982527, 13001184481914494968, 10540221995336646101, 7023240395126053547, 14176371989245961481, 5014329928124813889, 16384815164713326179, 3085626030079750855, 18320585783464736191, 8948575330343859216, 12505202744369394087, 16743767125120415198, 5984461727530517106, 4593123540345643871, 1783868876390637678, 12141628095852667086},
		{2534394130072522643, 11513502209579334080, 15658614040605937868, 3917537506837261411, 11497429918941053508, 2712699854887027443, 7954557426173635138, 3521474180128573187, 7485151044005712272, 8942920871232000588, 16032611571284264885, 16672774551201978867, 14536853662843482486, 103464190203496473, 7353530198730782131, 8905145790210487473, 916844908468858492},
		{9947059449668714493, 16205887618861664185, 10113421970943970917, 17861635849497181697, 211126607004828568, 14261576401408265472, 1116074565317489624, 8074610768302466720, 6261514658734514229, 11378080601754920890, 14137974802275453141, 10738028362812359243, 11633379704184351263, 15011813569203902071, 11665168795670307192, 11168990553799816012, 17244759220415138629},
		{14803590213734749977, 3452691610489420226, 16196772898428414283, 2779541700611411576, 3744707139821181422, 13208473139046919327, 18294611834926110324, 6889001185405029332, 2145400234106552813, 12040471266100107694, 3421875038315632063, 13803134548328045775, 3347162801830594768, 10002656615527119793, 3768059928286205882, 14505307313762064417, 2884471715135320549},
		{13332941379604641895, 7099643160491972031, 10350301900682509706, 2854773080333761175, 10887337704967921146, 14968782470006344437, 15096711108314750081, 4910493931410374225, 16497714834784255973, 6595408310998344150, 7015430323745865531, 16440941511535898324, 7614493191762940981, 6278994518913026314, 484077611659385351, 5422158640940323036, 12022301626212282247},
		{254716483887653531, 238330573977771559, 141327981851695105, 6007085980445691811, 14709098270914366542, 9770627907404734628, 8766201604613132003, 11175062448147803436, 2655085921513327522, 10372849124092910638, 9077424976478915659, 13260521506064966956, 16298313650751125181, 14477553881967717833, 6754340596029977738, 14447543829060991141, 14944218905702168276},
		{7026419523790238189, 7637127280420213948, 8171249354530903168, 13870757899376910863, 8244959061413488569, 6867350562533400655, 7148885067261848234, 1927091571594217547, 12714874861156240581, 6053670313571325841, 5701729849400563212, 7425690006529638514, 5318054718934059520, 16927151022417831214, 14330656221713755988, 17340375122237147279, 10820456951647858841},
		{14060099389427327460, 563995029633235363, 1473872684928149486, 5922766259533333176, 13905974306208152023, 15581499810137393099, 16984899920938428497, 3200603207280494362, 7380999954652714080, 12309868366858318294, 14429727436216005299, 2729787181478763922, 16977038330684590647, 18355554889446193926, 17215062679767734078, 13765200025381434216, 10344783318856046900},
		{9649735934438430274, 3399138371047184399, 398547652384713498, 5380614692813128343, 5411506409235652, 269352180507144378, 10455025401711960957, 22238466909932841, 1298765291149217147, 9161446061732264458, 1239012157003154588, 15005633193548090046, 14037255498647952574, 18039232131292507005, 16769083068199157833, 14036176587091740987, 16187922316611872964}
	};

public:
	AGobangFunction();

	UFUNCTION(BlueprintCallable)
	void Initialize();

	UFUNCTION(BlueprintCallable)
	int32 GetTurn();

	vector<vector<int32>> GetBoard();

	stack<pair<int32, int32>> GetRecord();

	UFUNCTION(BlueprintCallable)
	void GetPrevPiece(int32& X, int32& Y);

	UFUNCTION(BlueprintCallable)
	int32 Check(int32 X, int32 Y);

	UFUNCTION(BlueprintCallable)
	int32 Drop(int32 X, int32 Y);

	UFUNCTION(BlueprintCallable)
	void Undo();

	UFUNCTION(BlueprintCallable)
	int32 Judge();

	pair<pair<int32, int32>, pair<int32, int32>> GetWinningPos();

	UFUNCTION(BlueprintCallable)
	void Next(int32& X, int32& Y);

private:
	int32 AlphaBeta(int32, int32, int32, pair<int32, int32>* = nullptr);

	int32 EvaluateBoard();

	int32 EvaluatePoint(pair<int32, int32>);

	vector<int32> GetLine(int32, pair<int32, int32>);

	static int32 AnalysisLine(const vector<int32>&, int32);

	static int32 AnalysisPoint_My(const vector<int32>&, int32);

	static int32 AnalysisPoint_Adversary(const vector<int32>&, int32);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
