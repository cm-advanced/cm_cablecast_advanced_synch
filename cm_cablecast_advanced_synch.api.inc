<?php

/**
 * @file
 * Cablecast API
 */

/**
 * Cablecast server query
 *
 * @param $method
 * @param array $data
 * @return mixed
 */
function cm_cablecast_server_query($method, $data) {

  if ($serverAddress = variable_get('cm_cablecast_server_address', NULL)) {
    try {
      $client = new SoapClient($serverAddress . "/CablecastWS/CablecastWS.asmx?WSDL", array('cache_wsdl' => 0));
      // perform query
      $return = $client->$method($data);

      return $return;

    } // end try

    catch (SoapFault $fault) {
      trigger_error("The following SOAP Error was generated, SOAP Fault: (faultcode: {$fault->faultcode}, faultstring: {$fault->faultstring})", E_USER_ERROR);
    }// end catch
  }
}

/**
 * @param $producer
 * @return int
 */
function hook_cm_cablecast_add_producer($user) {
  /*
      <LocationID>int</LocationID>
      <ProducerName>string</ProducerName>
      <Phone1>string</Phone1>
      <Phone2>string</Phone2>
      <Address>string</Address>
      <Notes>string</Notes>
      <Active>boolean</Active>
      <ContactName>string</ContactName>
      <Email>string</Email>
      <Website>string</Website>
      <username>string</username>
      <password>string</password>
  */

  // setup data array for SOAP call
  $data = array(
    'LocationID'    => 1,
    'ProducerName'  => $user->display_name,
    'Phone1'        => '',
    'Phone2'        => '',
    'Address'       => '',
    'Notes'         => '',
    'Active'        => 1,
    'ContactName'   => $user->display_name,
    'Email'         => $user->mail,
    'Website'       => '',
    'username'      => variable_get('cm_cablecast_server_username', NULL),
    'password'      => variable_get('cm_cablecast_server_password', NULL),
  );

  $producer_id = cm_cablecast_server_query('CreateNewProducer', $data);

  return $producer_id->CreateNewProducerResult;

}

/**
 * @param $producer
 * @return int
 */
function hook_cm_cablecast_update_producer($user) {
  /*
      <ProducerID>int</ProducerID>
      <ProducerName>string</ProducerName>
      <Phone1>string</Phone1>
      <Phone2>string</Phone2>
      <Address>string</Address>
      <Notes>string</Notes>
      <Active>boolean</Active>
      <ContactName>string</ContactName>
      <Email>string</Email>
      <Website>string</Website>
      <username>string</username>
      <password>string</password>
  */

  // setup data array for SOAP call
  $data = array(
    'ProducerID'    => $user->field_cablecast_producer_id[LANGUAGE_NONE][0]['value'],
    'ProcuerName'   => $user->display_name,
    'Phone1'        => '',
    'Phone2'        => '',
    'Address'       => '',
    'Notes'         => '',
    'Active'        => 1,
    'ContactName'   => $user->display_name,
    'Email'         => $user->mail,
    'Website'       => '',
    'username'      => variable_get('cm_cablecast_server_username', NULL),
    'password'      => variable_get('cm_cablecast_server_password', NULL),
  );

  $producer_id = cm_cablecast_server_query('UpdateProducer', $data);

  return $producer_id->UpdateProducerResult;
}

/**
 * @param $producer
 * @return int
 */
function hook_cm_cablecast_get_producer($user) {
  /*
      <ProducerID>int</ProducerID>
      <username>string</username>
      <password>string</password>
  */

  // setup data array for SOAP call
  $data = array(
    'ProducerID'    => $user->field_cablecast_producer_id[LANGUAGE_NONE][0]['value'],
    'username'      => variable_get('cm_cablecast_server_username', NULL),
    'password'      => variable_get('cm_cablecast_server_password', NULL),
  );

  $producer_id = cm_cablecast_server_query('GetProducerInfo', $data);

  return $producer_id->GetProducerInfoResult->ProducerID;

}

/**
 * @param $node
 * @return int
 */
function hook_cm_cablecast_add_project($node, $producer_id) {
 /*
   <LocationID>int</LocationID>
   <Name>string</Name>
   <Description>string</Description>
   <ProducerID>int</ProducerID>
   <Podcast>boolean</Podcast>
   <PodcastName>string</PodcastName>
   <PodcastDescription>string</PodcastDescription>
   <username>string</username>
   <password>string</password>
 */


  // setup data array for SOAP call
  $data = array(
    'LocationID'          => 1,
    'Name'                => $node->title,
    'Description'         => '',
    'ProducerID'          => $producer_id,
    'Podcast'             => FALSE,
    'PodcastName'         => '',
    'PodcastDescription'  => '',
    'username'            => variable_get('cm_cablecast_server_username', NULL),
    'password'            => variable_get('cm_cablecast_server_password', NULL),
  );

  $project_id = cm_cablecast_server_query('CreateNewProject', $data);

  return $project_id->CreateNewProjectResult;
}

/**
 * @param $node
 * @return int
 */
function hook_cm_cablecast_update_project($node, $producer_id) {
  /*
    <ProjectID>int</ProjectID>
    <Name>string</Name>
    <Description>string</Description>
    <ProducerID>int</ProducerID>
    <Podcast>boolean</Podcast>
    <PodcastName>string</PodcastName>
    <PodcastDescription>string</PodcastDescription>
    <username>string</username>
    <password>string</password>
  */

  $cc_project = hook_cm_cablecast_get_project($node->field_cablecast_project_id[LANGUAGE_NONE][0]['value']);

  // setup data array for SOAP call
  $data = array(
    'ProjectID'           => $node->field_cablecast_project_id[LANGUAGE_NONE][0]['value'],
    'Name'                => $node->title,
    'Description'         => '',
    'ProducerID'          => $producer_id,
    'Podcast'             => FALSE,
    'PodcastName'         => '',
    'PodcastDescription'  => '',
    'username'            => variable_get('cm_cablecast_server_username', NULL),
    'password'            => variable_get('cm_cablecast_server_password', NULL),
  );
  $project_id = cm_cablecast_server_query('UpdateProject', $data);

  return $project_id->UpdateProjectResult;
}

/**
 * @param $node
 * @return int
 */
function hook_cm_cablecast_get_project($node) {
  /*
    <ChannelID>int</ChannelID>
  */

  $project_id = $node->field_cablecast_project_id[LANGUAGE_NONE][0]['value'];

  // setup data array for SOAP call
  $data = array(
    'ChannelID' => 1,
  );

  $projects = cm_cablecast_server_query('GetProjects', $data);

  // return project_id if the Project is found
  foreach ($projects->GetProjectsResult->Project as $project) {
    if ($project->ProjectID == $project_id) {
      return $project;
    }
  }

  return FALSE;
}

/**
 * @param $node
 * @return int
 */
function hook_cm_cablecast_add_show($node, $producer_id, $project_id) {
  /*
      <LocalID>string</LocalID>
      <LocationID>int</LocationID>
      <Title>string</Title>
      <CGTitle>string</CGTitle>
      <ProjectID>int</ProjectID>
      <CGExempt>boolean</CGExempt>
      <ProducerID>int</ProducerID>
      <Reels>
        <NewReel>
          <FormatID>int</FormatID>
          <CueSeconds>int</CueSeconds>
          <LengthSeconds>int</LengthSeconds>
          <Title>int</Title>
          <Chapter>int</Chapter>
        </NewReel>
        <NewReel>
          <FormatID>int</FormatID>
          <CueSeconds>int</CueSeconds>
          <LengthSeconds>int</LengthSeconds>
          <Title>int</Title>
          <Chapter>int</Chapter>
        </NewReel>
      </Reels>
      <CategoryID>int</CategoryID>
      <EventDate>dateTime</EventDate>
      <Comments>string</Comments>
      <CustomFields>
        <CustomField>
          <Name>string</Name>
          <Value>string</Value>
        </CustomField>
        <CustomField>
          <Name>string</Name>
          <Value>string</Value>
        </CustomField>
      </CustomFields>
      <OnDemand>boolean</OnDemand>
      <OnDemandStatus>string</OnDemandStatus>
      <BugText>string</BugText>
      <CrawlText>string</CrawlText>
      <CrawlLengthInSeconds>int</CrawlLengthInSeconds>
      <username>string</username>
      <password>string</password>
   */

  // All Cablecast shows need at least 1 Reel
  $reels = new stdClass();
  $reels->NewReel = new stdClass();
  $reels->NewReel->FormatID = 1;
  // CueSeconds
  if (isset($node->field_in_point[LANGUAGE_NONE][0]['value'])) {
    $reels->NewReel->CueSeconds = $node->field_in_point[LANGUAGE_NONE][0]['value'];
  } else {
    $reels->NewReel->CueSeconds = 0;
  }
  // LengthSeconds
  if (isset($node->field_runtime[LANGUAGE_NONE][0]['value'])) {
    $reels->NewReel->LengthSeconds = $node->field_runtime[LANGUAGE_NONE][0]['value'];
  } else {
    $reels->NewReel->LengthSeconds = 0;
  }
  $reels->NewReel->Title = 1;
  $reels->NewReel->Chapter = 1;

  // setup data array for SOAP call
  $data = array(
    'LocalID'               => "Added by API",
    'LocationID'            => 1,
    'Title'                 => $node->title,
    'CGTitle'               => $node->title,
    'ProjectID'             => $project_id,
    'CGExempt'              => FALSE, // TODO: may vary based on project type or other criteria
    'ProducerID'            => $producer_id,
    'Reels'                 => $reels,
    'CategoryID'            => 1,
    'EventDate'             => $time = date("c", time()),
    'Comments'              => '',
    'OnDemand'              => FALSE,
    'OnDemandStatus'        => FALSE,
    'CustomFields'          => FALSE,
    'BugText'               => '',
    'CrawlText'             => '',
    'CrawlLengthInSeconds'  => '',
    'username'              => variable_get('cm_cablecast_server_username', NULL),
    'password'              => variable_get('cm_cablecast_server_password', NULL),
  );

  $show_id = cm_cablecast_server_query('CreateNewShowRecord', $data);

  return $show_id->CreateNewShowRecordResult;
}

/**
 * @param $node
 * @param $cc_show (the original cablecast show to update)
 * @param $project_id
 * @param $producer_id
 *
 * @return int
 */
function hook_cm_cablecast_update_show($node, $cc_show, $project_id, $producer_id) {
  /*
    <ShowID>int</ShowID>
    <LocalID>string</LocalID>
    <Title>string</Title>
    <CGTitle>string</CGTitle>
    <ProjectID>int</ProjectID>
    <CGExempt>boolean</CGExempt>
    <ProducerID>int</ProducerID>
    <CategoryID>int</CategoryID>
    <EventDate>dateTime</EventDate>
    <Comments>string</Comments>
    <OnDemand>boolean</OnDemand>
    <OnDemandStatus>string</OnDemandStatus>
    <BugText>string</BugText>
    <CrawlText>string</CrawlText>
    <CrawlLengthInSeconds>int</CrawlLengthInSeconds>
    <username>string</username>
    <password>string</password>
   */

  // setup data array for SOAP call
  $data = array(
    'ShowID'                => $cc_show->ShowID,
    'LocalID'               => "Updated by API",
    'LocationID'            => 1,
    'Title'                 => $cc_show->InternalTitle,
    'CGTitle'               => $node->title,
    'ProjectID'             => $project_id,
    'CGExempt'              => $cc_show->CGExempt,
    'ProducerID'            => $producer_id,
    'CategoryID'            => $cc_show->CategoryID,
    'EventDate'             => $cc_show->EventDate,
    'Comments'              => $cc_show->Comments,
    'OnDemand'              => $cc_show->OnDemand,
    'OnDemandStatus'        => "",
    'CustomFields'          => $cc_show->CustomFields,
    'BugText'               => $cc_show->BugText,
    'CrawlText'             => $cc_show->CrawlText,
    'CrawlLengthInSeconds'  => $cc_show->CrawlLengthInSeconds,
    'username'              => variable_get('cm_cablecast_server_username', NULL),
    'password'              => variable_get('cm_cablecast_server_password', NULL),
  );


  $show_id = cm_cablecast_server_query('UpdateShowRecord', $data);

  return $show_id->UpdateShowRecordResult;
}

/**
 * @param $node
 * @return int
 */
function hook_cm_cablecast_get_show($node) {
  /*
    <ShowID>int</ShowID>
   */

  // setup data array for SOAP call
  $data = array(
    'ShowID'                => $node->field_cablecast_show_id[LANGUAGE_NONE][0]['value'],
  );


  $show = cm_cablecast_server_query('GetShowInformation', $data);

  return $show->GetShowInformationResult;
}
