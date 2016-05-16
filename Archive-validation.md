Archive validation commands (--validate)
This command helps to maintain the integrity of the mirror by scanning each file in the archive and comparing it with a list of checksums that validate that the files are not corrupted or missing. Because the archive contains images that may not be accessed for some time images, sometimes months, even years, these image can go missing or get corrupted without you knowing for some time. This can be done by accident or maliciously. However the damage is done and the longer left untreated the more likely that the repair will be harder as the backup may be harder to find. The process of validating the archive may take some time to scan the entire archive so this command allows you to be selective of what is to be scanned.   
--all
--archive
--view
--mirror
