#include "net.h"
#include "masternodeconfig.h"
#include "util.h"
#include "chainparams.h"
#include <base58.h>

CMasternodeConfig masternodeConfig;


void CMasternodeConfig::add(std::string alias, std::string ip, std::string privKey, std::string txHash, std::string outputIndex, std::string rewardAddress, std::string rewardPercent) {
    CMasternodeEntry cme(alias, ip, privKey, txHash, outputIndex, rewardAddress, rewardPercent);
    entries.push_back(cme);
}

bool CMasternodeConfig::read(boost::filesystem::path path) {
    boost::filesystem::ifstream streamConfig(GetMasternodeConfigFile());
    if (!streamConfig.good()) {
        return true; // No masternode.conf file is OK
    }

    for(std::string line; std::getline(streamConfig, line); )
    {
        if(line.empty()) {
            continue;
        }
        std::istringstream iss(line);
        std::string alias, ip, privKey, txHash, outputIndex, reward, rewardAddress, rewardPercent;
        if (!(iss >> alias >> ip >> privKey >> txHash >> outputIndex >> reward)) {
            rewardAddress = "";
            rewardPercent = "";
            iss.str(line);
            iss.clear();
            if (!(iss >> alias >> ip >> privKey >> txHash >> outputIndex)) {
                LogPrintf("Could not parse masternode.conf line: %s\n", line.c_str());
                streamConfig.close();
                return false;
            }
        } else {
            size_t pos = reward.find_first_of(":");
            if(pos == string::npos) { // no ":" found
                rewardPercent = "100";
                rewardAddress = reward;
            } else {
                rewardPercent = reward.substr(pos + 1);
                rewardAddress = reward.substr(0, pos);
            }
            CBitcoinAddress address(rewardAddress);
            if (!address.IsValid()) {
                LogPrintf("Invalid TX address in masternode.conf line: %s\n", line.c_str());
                streamConfig.close();
                return false;
            }
        }

        if(Params().NetworkID() == CChainParams::MAIN){
            if(CService(ip).GetPort() != Params().GetDefaultPort()) {
                LogPrintf("Invalid port detected in masternode.conf: %s (must be 11368 for mainnet)\n", line.c_str());
                streamConfig.close();
                return false;
            }
        } else if(CService(ip).GetPort() == Params().GetDefaultPort()) {
            LogPrintf("Invalid port detected in masternode.conf: %s (11368 must be only on mainnet)\n", line.c_str());
            streamConfig.close();
            return false;
        }

        if (!(CService(ip).IsIPv4() && CService(ip).IsRoutable())) {
            LogPrintf("Invalid Address detected in masternode.conf: %s (IPV4 ONLY) \n", line.c_str());
            streamConfig.close();
            return false;
        }

        add(alias, ip, privKey, txHash, outputIndex, rewardAddress, rewardPercent);
    }

    streamConfig.close();
    return true;
}
