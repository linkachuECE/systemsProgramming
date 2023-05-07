static struct nf_hook_ops filterHook;

int setUpFilter(void){
    filterHook.hook = block;
    filterHook.hooknum = NF_INET_POST_ROUTING;
    filterHook.pf = PF_INET;
    filterHook.priority = NF_IP_PRI_FIRST;
    nf_register_hook(&filterHook);
    return 0;
}

void removeFilter(void){
    nf_unregister_hook(&filterHook);
}

module_init(setUpFilter);
module_exit(removeFilter);

unsigned int block(void *priv, struct sk_buff *skb,
const struct nf_hook_state *state) {
    if(!skb){
        printk(KERN_INFO,"packet receive not correct\n");
        return NF_DROP;
    }

    struct iphdr *iph;
    struct tcphdr *tcph;

    iph = ip_hdr(@@@@@@);
    tcph = (void *)iph+iph->ihl*4;

    __u32 sou_ip = iph->saddr;
    __u32 des_ip = iph->daddr;
    __u16 sou_port = tcph->source;
    __u16 des_port = tcph->dest;

    if(des_ip==in_aton("@@@@@@") && ntohs(des_port)== @@@@@@) {
        return @@@@@@;
    }

    return NF_ACCEPT;
}